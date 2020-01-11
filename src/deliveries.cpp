#include "tracelytics/tracelytics.hpp"

/**
 * Create New delivery
 **/
void tracelytics::newdelivery (
    const std::string& user,
    const std::string& company,
    const std::string& deliveryId,
    const std::string& route,
    const std::string& fromSite,
    const std::string& toSite,
    const std::string& fromCompany,
    const std::string& toCompany,
    const time_point& startTime,
    const std::string& type,
    const std::map<std::string, ProductQuantity>& cargo,
    const time_point& timestamp,
    const std::map<std::string, all_type>& data,

    const optional<time_point>& endTime,
    const optional<std::string>& shipper,
    const optional<std::string>& driver,
    const optional<std::string>& status,
    const optional<std::string>& description,
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),        "user is missing.");
    check(!company.empty(),     "company is missing.");
    check(!deliveryId.empty(),  "delivery ID is missing.");
    // check(!route.empty(),      "route is missing."); // Can be empty
    check(!fromSite.empty(),    "from site is missing.");
    check(!toSite.empty(),      "to site is missing.");
    check(!fromCompany.empty(), "sending company is missing.");
    check(!toCompany.empty(),   "receiving company is missing.");
    check(company == fromCompany || company == toCompany, "must be part of sending or receiving company."); // IMPORTANT
    check(type == DeliveryActions::SEND_DELIVERY || type == DeliveryActions::RECEIVE_DELIVERY, "must send or receive."); // IMPORTANT

    // Access table and make sure delivery doesnt exist
    auto deliveries_byid = _deliveries.get_index<eosio::name("byroute")>();
    auto delivery = deliveries_byid.find(Checksum::DELIVERY(deliveryId, route));
    check(delivery == deliveries_byid.end(), "delivery already exists");

    // Create new delivery
    _deliveries.emplace(get_self(), [&](auto& d) {
        d.index     = _deliveries.available_primary_key();
        d.createdBy = user;
        d.updatedBy = user;
        d.createdAt = timestamp;
        d.updatedAt = timestamp;

        d.deliveryId  = deliveryId;
        d.route       = route;
        d.fromSite    = fromSite;
        d.toSite      = toSite;
        d.fromCompany = fromCompany;
        d.toCompany   = toCompany;
        d.startTime   = startTime;
        d.type        = type;
        d.cargo       = cargo;

        // Optional
        if (endTime)     d.endTime     = *endTime;
        if (shipper)     d.shipper     = *shipper;
        if (driver)      d.driver      = *driver;
        if (status)      d.status      = *status;
        if (description) d.description = *description;
        if (version)     d.version     = *version;

        // Check sites exist
        check_site_exists(fromCompany, fromSite);
        auto site = check_site_exists(toCompany, toSite);

        // If not tracked, or receiving delivery (Magic delivery)
        if (!site.tracked || type == DeliveryActions::RECEIVE_DELIVERY) {
            d.status = DeliveryStatus::DELIVERED;
            if (!endTime) {
                d.endTime = timestamp;
            }
        }

        // SENDING delivery, so we substract inventory from our site
        std::map<std::string, ProductQuantity> emptyDelta;
        processcargo<Delivery>(d, d.cargo, emptyDelta, user, company, Actions::NEW_DELIVERY, type, false);
    });
}

/**
 * Edit delivery
 **/
void tracelytics::editdelivery (
    const std::string& user,
    const std::string& company,
    const std::string& deliveryId,
    const std::string& route,
    const std::map<std::string, ProductQuantity>& cargoDeltas,
    const time_point& timestamp,
    const std::map<std::string, all_type>& data,

    const optional<std::string>& toSite,
    const optional<std::string>& toCompany,
    const optional<time_point>&  startTime,
    const optional<time_point>&  endTime,
    const optional<std::string>& shipper,
    const optional<std::string>& driver,
    const optional<std::string>& status,
    const optional<std::string>& description,
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),       "user is missing.");
    check(!company.empty(),    "company is missing.");
    check(!deliveryId.empty(), "delivery ID is missing.");
    // check(!route.empty(),      "route is missing."); // Can be empty

    // Access table and make sure delivery exists
    auto deliveries_byid = _deliveries.get_index<eosio::name("byroute")>();
    auto delivery = deliveries_byid.find(Checksum::DELIVERY(deliveryId, route));
    check(delivery != deliveries_byid.end(), "delivery does not exist.");
    check(deliveryId == delivery->deliveryId && route == delivery->route, "delivery mismatch");
    check(company == delivery->fromCompany || company == delivery->toCompany, "must be part of sending or receiving company."); // IMPORTANT

    // Edit delivery
    deliveries_byid.modify(delivery, get_self(), [&](auto& d) {
        d.updatedBy = user;
        d.updatedAt = timestamp;

        // Checks: 1) New status is DELIVERED and old status is not DELIVERED
        bool justDelivered = status.value_or("") == DeliveryStatus::DELIVERED && d.status != DeliveryStatus::DELIVERED;

        // Optional
        if (shipper)     d.shipper     = *shipper;
        if (driver)      d.driver      = *driver;
        if (startTime)   d.startTime   = *startTime;
        if (endTime)     d.endTime     = *endTime;
        if (status)      d.status      = *status;
        if (toSite)      d.toSite      = *toSite;
        if (toCompany)   d.toCompany   = *toCompany;
        if (description) d.description = *description;
        if (version)     d.version     = *version;

        // TODO Check if we want to allow editing and delivering in same call
        if (cargoDeltas.size() > 0) {
            processcargo<Delivery>(d, d.cargo, cargoDeltas, user, company, Actions::EDIT_DELIVERY, DeliveryActions::EDIT_CARGO, false);
        }

        // Process delivery
        if (justDelivered) {
            std::map<std::string, ProductQuantity> emptyCargoDeltas;
            processcargo<Delivery>(d, d.cargo, emptyCargoDeltas, user, company, Actions::EDIT_DELIVERY, DeliveryActions::RECEIVE_DELIVERY, false);
        }
    });
}

/**
 * Delete delivery
 **/
void tracelytics::deldelivery (
    const std::string& user,
    const std::string& company,
    const std::string& deliveryId,
    const std::string& route,
    const time_point&  timestamp,
    const bool& cancel
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),       "user is missing.");
    check(!company.empty(),    "company is missing.");
    check(!deliveryId.empty(), "delivery ID is missing.");
    // check(!route.empty(),      "route is missing."); // Can be empty

    // Access table and make sure delivery exists
    auto deliveries_byid = _deliveries.get_index<eosio::name("byroute")>();
    auto delivery = deliveries_byid.find(Checksum::DELIVERY(deliveryId, route));
    check(delivery != deliveries_byid.end(), "delivery does not exist.");
    check(deliveryId == delivery->deliveryId && route == delivery->route, "delivery mismatch");

    if (user != ADMIN) {
        check(company == delivery->fromCompany, "the user must be a part of the sending company.");
        check(delivery->status != DeliveryStatus::DELIVERED, "cannot cancel or delete a delivered delivery");
        check(delivery->status != DeliveryStatus::CANCELLED, "cannot cancel or delete a cancelled delivery");
    }

    if (cancel) {
        deliveries_byid.modify(delivery, get_self(), [&](auto& d) {
            d.updatedBy = user;
            d.updatedAt = timestamp;
            d.status = DeliveryStatus::CANCELLED;

            // Refund
            std::map<std::string, ProductQuantity> emptyDeltas;
            processcargo(d, d.cargo, emptyDeltas, user, company, Actions::DELETE_DELIVERY, DeliveryActions::CANCEL_DELIVERY, false);
        });
    } else {
        deliveries_byid.erase(delivery);
    }
}
