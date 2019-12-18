#include "tracelytics/tracelytics.hpp"

// TODO change all times from string to time point

/**
 * Create New delivery
 *
 * Params:
 *  - company (name)
 *  - deliveryId (string)
 *  - startTime (string)
 *  - endTime (string) (optional)
 *  - shipperId (string) (optional)
 *  - driverId (string) (optional)
 *  - status (string) (optional)
 **/
void tracelytics::newdelivery (
    std::map<std::string, all_type> args,
    std::map<checksum256, Cargo> cargo
) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company    =  std::get_if<name>       (  &args["company"]   );
    auto deliveryId =  std::get_if<std::string>(  &args["deliveryId"]);
    auto startTime  =  std::get_if<std::string>(  &args["startTime"] );
    auto endTime    =  std::get_if<std::string>(  &args["endTime"]   ); // Optional
    auto shipperId  =  std::get_if<std::string>(  &args["shipperId"] ); // Optional
    auto driverId   =  std::get_if<std::string>(  &args["driverId"]  ); // Optional
    auto status     =  std::get_if<std::string>(  &args["status"]    ); // Optional

    // Validation
    check(company    != nullptr, "company is missing.");
    check(deliveryId != nullptr, "delivery id is missing.");
    check(startTime  != nullptr, "start time is missing.");
    check(cargo.size() > 0, "cargo is empty.");

    // Generate checksum from deliveryId
    checksum256 delivery_checksum = sha256(deliveryId->c_str(), deliveryId->size());

    // Access table and make sure delivery doesnt exist
    delivery_table deliveries(get_self(), company->value);
    auto deliveries_bychecksum = deliveries.get_index<"bychecksum"_n>();
    auto delivery = deliveries_bychecksum.find(delivery_checksum);
    check(delivery == deliveries_bychecksum.end(), "delivery already exists");

    // Create new delivery
    deliveries.emplace(get_self(), [&](auto& d) {
        d.id = deliveries.available_primary_key();
        d.delivery_checksum = delivery_checksum;
        d.delivery_id = *deliveryId;
        d.start_time = *startTime;
        d.cargo = cargo;

        // Optional
        if (endTime   != nullptr) d.end_time    = *endTime;
        if (shipperId != nullptr) d.shipper_id  = *shipperId;
        if (driverId  != nullptr) d.driver_id   = *driverId;
        if (status    != nullptr) d.status      = *status;
    });
}

/**
 * Edit delivery
 *
 * Args Params:
 *  - company (name)
 *  - deliveryId (string)
 *  - shipperId (string) (optional)
 *  - driverId (string) (optional)
 *  - startTime (string) (optional)
 *  - endTime (string) (optional)
 *  - status (string) (optional)
 **/
void tracelytics::editdelivery (
    std::map<std::string, all_type> args,
    std::map<checksum256, Cargo> cargodelta
) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company    =  std::get_if<name>       (  &args["company"]    );
    auto deliveryId =  std::get_if<std::string>(  &args["deliveryId"] );
    auto shipperId  =  std::get_if<std::string>(  &args["shipperId"]  ); // Optional
    auto driverId   =  std::get_if<std::string>(  &args["driverId"]   ); // Optional
    auto startTime  =  std::get_if<std::string>(  &args["startTime"]   ); // Optional
    auto endTime    =  std::get_if<std::string>(  &args["endTime"]    ); // Optional
    auto status     =  std::get_if<std::string>(  &args["status"]     ); // Optional

    // Validation
    check(company != nullptr, "company is missing.");
    check(deliveryId != nullptr, "delivery id is missing.");

    // Generate checksum from deliveryId
    checksum256 delivery_checksum = sha256( deliveryId->c_str(), deliveryId->size() );

    // Access table and make sure delivery exists
    delivery_table deliveries(get_self(), company->value);
    auto deliveries_bychecksum = deliveries.get_index<"bychecksum"_n>();
    auto delivery = deliveries_bychecksum.find( delivery_checksum );
    check(delivery != deliveries_bychecksum.end(), "delivery does not exist.");

    // For each cargo
    for( auto const& [productInstanceChecksum, Cargo] : cargodelta ) {
        product_instance_table productInstances(get_self(), company->value);
        auto productInstancesByChecksum = productInstances.get_index<"bychecksum"_n>();
        auto productInstance = productInstancesByChecksum.find( productInstanceChecksum );
        check(productInstance != productInstancesByChecksum.end(), "product instance does not exist.");


    }

    // Edit delivery
    deliveries_bychecksum.modify(delivery, get_self(), [&](auto& d) {
        // Optional
        if (shipperId      != nullptr) d.shipper_id    = *shipperId;
        if (driverId       != nullptr) d.driver_id     = *driverId;
        if (startTime      != nullptr) d.start_time    = *startTime;
        if (endTime        != nullptr) d.end_time      = *endTime;
        if (status         != nullptr) d.status        = *status;
    });
}

/**
 * Delete delivery
 *
 * Params:
 *  - company (name)
 *  - deliveryId (string)
 **/
void tracelytics::deldelivery (std::map<std::string, all_type> args) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company    = std::get_if<name>       (  &args["company"]   );
    auto deliveryId = std::get_if<std::string>(  &args["deliveryId"]);

    // Validation
    check(company != nullptr, "company is missing.");
    check(deliveryId != nullptr, "delivery id is missing.");

    // Generate checksum from deliveryId
    checksum256 delivery_checksum = sha256(deliveryId->c_str(), deliveryId->size());

    // Access table and make sure delivery exists
    delivery_table deliveries(get_self(), company->value);
    auto deliveries_bychecksum = deliveries.get_index<"bychecksum"_n>();
    auto delivery = deliveries_bychecksum.find(delivery_checksum);
    check(delivery != deliveries_bychecksum.end(), "delivery does not exist.");

    // Delete delivery
    deliveries_bychecksum.erase(delivery);
}
