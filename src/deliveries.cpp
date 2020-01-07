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
    check(!deliveryId.empty(),  "delivery id is missing.");
    // check(!route.empty(),      "route is missing."); // Can be empty
    check(!fromSite.empty(),    "from site is missing.");
    check(!toSite.empty(),      "to site is missing.");
    check(!fromCompany.empty(), "sending company is missing.");
    check(!toCompany.empty(),   "receiving company is missing.");
    check(company == fromCompany || company == toCompany, "must be part of sending or receiving company."); // IMPORTANT

    // Generate checksum from deliveryId
    std::string delivery_checksum_string = deliveryId + ";" + route;
    checksum256 delivery_checksum = sha256(delivery_checksum_string.c_str(), delivery_checksum_string.size());

    // Access table and make sure delivery doesnt exist
    auto deliveries_byid = _deliveries.get_index<eosio::name("byroute")>();
    auto delivery = deliveries_byid.find(delivery_checksum);
    check(delivery == deliveries_byid.end(), "delivery already exists");

    // Create new delivery
    _deliveries.emplace(get_self(), [&](auto& d) {
      d.index      = _deliveries.available_primary_key();
      d.createdBy  = user;
      d.updatedBy  = user;
      d.createdAt  = timestamp;
      d.updatedAt  = timestamp;

      d.deliveryId  = deliveryId;
      d.route       = route;
      d.fromSite    = fromSite;
      d.toSite      = toSite;
      d.fromCompany = fromCompany;
      d.toCompany   = toCompany;
      d.startTime   = startTime;
      d.cargo       = cargo;

      // Optional
      if (endTime)     d.endTime     = *endTime;
      if (shipper)     d.shipper     = *shipper;
      if (driver)      d.driver      = *driver;
      if (status)      d.status      = *status;
      if (description) d.description = *description;
      if (version)     d.version     = *version;

      // Optimization to end delivery if RECEIVING
      // ~~~~~ MAGIC MOCK DELIVERY ~~~~~~~~~~~
      // We dont charge the fromSite in this case
      if (company == toCompany) {
        d.status = DeliveryStatus::DELIVERED;

        if (!endTime) {
            d.endTime = timestamp;
        }
      }

      // SENDING delivery, so we substract inventory from our site
      std::map<std::string, ProductQuantity> emptyDelta;
      processcargo(d, d.cargo, emptyDelta, user, company, Actions::NEW_DELIVERY, false);
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
    check(!deliveryId.empty(), "delivery id is missing.");
    // check(!route.empty(),      "route is missing."); // Can be empty

    // Access table and make sure delivery exists
    auto deliveries_byid = _deliveries.get_index<eosio::name("byroute")>();
    auto delivery = deliveries_byid.find(SHA256( company + ";" + deliveryId ));
    check(delivery != deliveries_byid.end(), "delivery does not exist.");
    check(deliveryId == delivery->deliveryId && route == delivery->route, "delivery mismatch");

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

        // Process edits
        bool needToEditCargo = cargoDeltas.size() > 0 && d.status != DeliveryStatus::DELIVERED;
        if (needToEditCargo) {
            processcargo(d, d.cargo, cargoDeltas, user, company, Actions::EDIT_DELIVERY, false);
        }

        // Process delivery
        if (justDelivered) {
            std::map<std::string, ProductQuantity> emptyCargo;
            processcargo(d, d.cargo, emptyCargo, user, company, Actions::EDIT_DELIVERY, false);
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
    const time_point&  timestamp
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),       "user is missing.");
    check(!company.empty(),    "company is missing.");
    check(!deliveryId.empty(), "delivery id is missing.");
    // check(!route.empty(),      "route is missing."); // Can be empty

    // Generate checksum from deliveryId
    std::string delivery_checksum_string = company + ";" + deliveryId;
    checksum256 delivery_checksum = sha256(delivery_checksum_string.c_str(), delivery_checksum_string.size());

    // Access table and make sure delivery exists
    auto deliveries_byid = _deliveries.get_index<eosio::name("byroute")>();
    auto delivery = deliveries_byid.find(delivery_checksum);
    check(delivery != deliveries_byid.end(), "delivery does not exist.");
    check(company == delivery->fromCompany || company == delivery->toCompany, "the user must be a part of the sending or receiving company.");
    check(deliveryId == delivery->deliveryId && route == delivery->route, "delivery mismatch");

    // TODO refund here

    // Delete delivery
    deliveries_byid.erase(delivery);
}
