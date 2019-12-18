#include "tracelytics/tracelytics.hpp"

/**
 * Create New Productins
 *
 * Params:
 *  - company (name)
 *  - productinsId (string)
 *  - product (string) (optional)
 **/
void tracelytics::newprodins (std::map<std::string, all_type> args) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company       = std::get_if<name>       (  &args["company"] );
    auto productinsId  = std::get_if<std::string>(  &args["productinsId"] );
    auto product       = std::get_if<std::string>(  &args["product"] );

    // Validation
    check(company != nullptr, "company is missing.");
    check(productinsId  != nullptr, "productins id is missing.");

    // Generate checksum from productinsId
    checksum256 productins_checksum = sha256(productinsId->c_str(), productinsId->size());

    // Access table and make sure productins doesnt exist
    product_instance_table product_instances(get_self(), company->value);
    auto productins_bychecksum = product_instances.get_index<"bychecksum"_n>();
    auto productins = productins_bychecksum.find(productins_checksum);
    check(productins == productins_bychecksum.end(), "product instance already exists");

    // Create new productins
    product_instances.emplace(get_self(), [&](auto& p) {
        p.id = product_instances.available_primary_key();
        p.productins_checksum = productins_checksum;
        p.productins_id = *productinsId;

        // Optional
        if (product != nullptr) p.product = *product;
    });
}

/**
 * Edit Productins
 *
 * Params:
 *  - company (name)
 *  - productinsId (string)
 *  - product (string) (optional)
 **/
void tracelytics::editprodins (std::map<std::string, all_type> args) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company      = std::get_if<name>       (  &args["company"] );
    auto productinsId = std::get_if<std::string>(  &args["productinsId"] );
    auto product      = std::get_if<std::string>(  &args["product"] );

    // Validation
    check(company != nullptr, "company is missing.");
    check(productinsId != nullptr, "productins id is missing.");

    // Generate checksum from productinsId
    checksum256 productins_checksum = sha256( productinsId->c_str(), productinsId->size() );

    // Access table and make sure productins exists
    product_instance_table product_instances(get_self(), company->value);
    auto productins_bychecksum = product_instances.get_index<"bychecksum"_n>();
    auto productins = productins_bychecksum.find( productins_checksum );
    check(productins != productins_bychecksum.end(), "productins does not exist.");

    // Edit Productins
    productins_bychecksum.modify(productins, get_self(), [&](auto& p) {
        // Optional
        if (product != nullptr) p.product = *product;
    });
}

/**
 * Delete Productins
 *
 * Params:
 *  - company (name)
 *  - productinsId (string)
 **/
void tracelytics::delprodins (std::map<std::string, all_type> args) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company      = std::get_if<name>       ( &args["company"] );
    auto productinsId = std::get_if<std::string>( &args["productinsId"] );

    // Validation
    check(company != nullptr, "company is missing.");
    check(productinsId != nullptr, "productins id is missing.");

    // Generate checksum from productinsId
    checksum256 productins_checksum = sha256(productinsId->c_str(), productinsId->size());

    // Access table and make sure productins exists
    product_instance_table product_instances(get_self(), company->value);
    auto productins_bychecksum = product_instances.get_index<"bychecksum"_n>();
    auto productins = productins_bychecksum.find(productins_checksum);
    check(productins != productins_bychecksum.end(), "productins does not exist.");

    // Delete productins
    productins_bychecksum.erase(productins);
}
