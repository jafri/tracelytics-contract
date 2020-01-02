#include "tracelytics/tracelytics.hpp"

/**
 * Create New delivery
 **/
void tracelytics::newdelivery (
    const std::string& user,
    const std::string& company,
    const std::string& deliveryId,
    const std::string& type,
    const std::string& fromSite,
    const std::string& toSite,
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
    check(!user.empty(),       "user is missing.");
    check(!company.empty(),    "company is missing.");
    check(!deliveryId.empty(), "delivery id is missing.");
    check(!fromSite.empty(),   "from site is missing.");
    check(!toSite.empty(),     "to site is missing.");
    check(type == "internal" || type == "external", "type must be internal or external");

    // Generate checksum from deliveryId
    std::string delivery_checksum_string = company + ";" + deliveryId;
    checksum256 delivery_checksum = sha256(delivery_checksum_string.c_str(), delivery_checksum_string.size());

    // Access table and make sure delivery doesnt exist
    delivery_table deliveries(get_self(), get_self().value);
    auto deliveries_bychecksum = deliveries.get_index<"bychecksum"_n>();
    auto delivery = deliveries_bychecksum.find(delivery_checksum);
    check(delivery == deliveries_bychecksum.end(), "delivery already exists");

    // If cargo is initialized, deduct quantity from site inventory
    if (cargo.size() > 0) {
        for( auto const& [item, productAndQuantity] : cargo ) {
            // Process item quantity change
            upsertitem(user,
                       company,
                       fromSite,
                       item,
                       productAndQuantity.product,
                      -productAndQuantity.quantity, // NEGATIVE
                       productAndQuantity.metadata,
                       (std::string) "newdelivery",
                       deliveryId,
                       timestamp);
        }
    }

    // Create new delivery
    deliveries.emplace(get_self(), [&](auto& d) {
        d.index      = deliveries.available_primary_key();
        d.createdBy  = user;
        d.updatedBy  = user;
        d.createdAt  = timestamp;
        d.updatedAt  = timestamp;

        d.company    = company;
        d.deliveryId = deliveryId;
        d.type       = type;
        d.fromSite   = fromSite;
        d.toSite     = toSite;
        d.startTime  = startTime;
        d.cargo      = cargo;

        // Optional
        if (endTime)     d.endTime     = *endTime;
        if (shipper)     d.shipper     = *shipper;
        if (driver)      d.driver      = *driver;
        if (status)      d.status      = *status;
        if (description) d.description = *description;
        if (version)     d.version     = *version;
    });
}


/**
 * Edit delivery
 **/
void tracelytics::editdelivery (
    const std::string& user,
    const std::string& company,
    const std::string& deliveryId,
    const std::map<std::string, ProductQuantity>& cargoDelta,
    const time_point& timestamp,
    const std::map<std::string, all_type>& data,

    const optional<std::string>& toSite,
    const optional<std::string>& type,
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

    // Generate checksum from deliveryId
    std::string delivery_checksum_string = company + ";" + deliveryId;
    checksum256 delivery_checksum = sha256(delivery_checksum_string.c_str(), delivery_checksum_string.size());

    // Access table and make sure delivery exists
    delivery_table deliveries(get_self(), get_self().value);
    auto deliveries_bychecksum = deliveries.get_index<"bychecksum"_n>();
    auto delivery = deliveries_bychecksum.find( delivery_checksum );
    check(delivery != deliveries_bychecksum.end(), "delivery does not exist.");

    // If the new status of delivery is "delivered"
    // AND not already delivered
    if (status.value_or("") == DeliveryStatus::Delivered && delivery->status != DeliveryStatus::Delivered) {
        // For each cargo
        for( auto const& [item, productAndQuantity] : delivery->cargo ) {
            upsertitem(user,
                       company,
                       delivery->toSite,
                       item,
                       productAndQuantity.product,
                       productAndQuantity.quantity,
                       productAndQuantity.metadata,
                       (std::string) "editdelivery",
                       deliveryId,
                       timestamp);
        }
    }

    // Edit delivery
    deliveries_bychecksum.modify(delivery, get_self(), [&](auto& d) {
        d.updatedBy = user;
        d.updatedAt = timestamp;

        // Optional
        if (shipper)     d.shipper     = *shipper;
        if (driver)      d.driver      = *driver;
        if (startTime)   d.startTime   = *startTime;
        if (endTime)     d.endTime     = *endTime;
        if (status)      d.status      = *status;
        if (toSite)      d.toSite      = *toSite;
        if (type)        d.type        = *type;
        if (description) d.description = *description;
        if (version)     d.version     = *version;

        // Can only edit the cargo BEFORE it is "delivered"
        if (cargoDelta.size() > 0 && d.status != DeliveryStatus::Delivered) {
            // Consistent
            auto* mapOfItems = &d.cargo;
            auto* mapOfDeltas = &cargoDelta;
            auto* row = &d;

            for( auto const& [item, productAndQuantity] : *mapOfDeltas ) {
                // Ensure new quantity is zero or positive
                check(productAndQuantity.quantity >= 0, "quantity in delivery must be zero or positive for item " + item);

                // processingDelta = new quantity of item - current quantity
                int64_t processingDelta = mapOfItems->count(item) > 0
                    ? productAndQuantity.quantity - (*mapOfItems)[item].quantity // In cargo
                    : productAndQuantity.quantity;                               // Not in cargo
                int64_t inventoryDelta = -processingDelta;

                /**
                 * inventoryDelta has 3 states
                 * Zero: No change
                 * Positive (+): - Cargo -> + Inventory (refund)
                 * Negative (-): - Inventory -> + Cargo (charge)
                 **/

                // Skip if processingDelta is 0, as nothing changes
                if (inventoryDelta == 0) {
                    continue;
                }

                // Update cargo
                if (productAndQuantity.quantity > 0) {
                    (*mapOfItems)[item] = productAndQuantity;
                } else if (productAndQuantity.quantity == 0) {
                    (*mapOfItems).erase(item);
                }

                // Process item quantity change
                upsertitem(user,
                           company,
                           row->fromSite,
                           item,
                           productAndQuantity.product,
                           inventoryDelta,
                           productAndQuantity.metadata,
                           (std::string) "editdelivery",
                           deliveryId,
                           timestamp);
            }
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
    const time_point&  timestamp
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),       "user is missing.");
    check(!company.empty(),    "company is missing.");
    check(!deliveryId.empty(), "delivery id is missing.");

    // Generate checksum from deliveryId
    std::string delivery_checksum_string = company + ";" + deliveryId;
    checksum256 delivery_checksum = sha256(delivery_checksum_string.c_str(), delivery_checksum_string.size());

    // Access table and make sure delivery exists
    delivery_table deliveries(get_self(), get_self().value);
    auto deliveries_bychecksum = deliveries.get_index<"bychecksum"_n>();
    auto delivery = deliveries_bychecksum.find(delivery_checksum);
    check(delivery != deliveries_bychecksum.end(), "delivery does not exist.");

    // Delete delivery
    deliveries_bychecksum.erase(delivery);
}
