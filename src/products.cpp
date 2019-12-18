#include "tracelytics/tracelytics.hpp"

/**
 * Create New Product
 *
 * Params:
 *  - company (name)
 *  - productId (string)
 *  - name (string) (optional)
 *  - description (string) (optional)
 **/
void tracelytics::newproduct (std::map<std::string, all_type> args) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company     = std::get_if<name>       (  &args["company"] );
    auto productId   = std::get_if<std::string>(  &args["productId"] );
    auto name        = std::get_if<std::string>(  &args["name"] );
    auto description = std::get_if<std::string>(  &args["description"] );

    // Validation
    check(company != nullptr, "company is missing.");
    check(productId != nullptr, "product id is missing.");

    // Generate checksum from productId
    checksum256 product_checksum = sha256(productId->c_str(), productId->size());

    // Access table and make sure product doesnt exist
    product_table products(get_self(), company->value);
    auto products_bychecksum = products.get_index<"bychecksum"_n>();
    auto product = products_bychecksum.find(product_checksum);
    check(product == products_bychecksum.end(), "product already exists");

    // Create new product
    products.emplace(get_self(), [&](auto& p) {
        p.id = products.available_primary_key();
        p.product_checksum = product_checksum;
        p.product_id = *productId;

        // Optional
        if (name != nullptr) p.name = *name;
        if (description != nullptr) p.description = *description;
    });
}

/**
 * Edit Product
 *
 * Params:
 *  - company (name)
 *  - productId (string)
 *  - name (string) (optional)
 **/
void tracelytics::editproduct (std::map<std::string, all_type> args) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company     = std::get_if<name>       (  &args["company"] );
    auto productId   = std::get_if<std::string>(  &args["productId"] );
    auto name        = std::get_if<std::string>(  &args["name"] );
    auto description = std::get_if<std::string>(  &args["description"] );

    // Validation
    check(company != nullptr, "company is missing.");
    check(productId != nullptr, "product id is missing.");

    // Generate checksum from productId
    checksum256 product_checksum = sha256( productId->c_str(), productId->size() );

    // Access table and make sure product exists
    product_table products(get_self(), company->value);
    auto products_bychecksum = products.get_index<"bychecksum"_n>();
    auto product = products_bychecksum.find( product_checksum );
    check(product != products_bychecksum.end(), "product does not exist.");

    // Edit Product
    products_bychecksum.modify(product, get_self(), [&](auto& p) {
        // Optional
        if (name != nullptr) p.name = *name;
        if (description != nullptr) p.description = *description;
    });
}

/**
 * Delete Product
 *
 * Params:
 *  - company (name)
 *  - productId (string)
 **/
void tracelytics::delproduct (std::map<std::string, all_type> args) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company   = std::get_if<name>       ( &args["company"] );
    auto productId = std::get_if<std::string>( &args["productId"] );

    // Validation
    check(company != nullptr, "company is missing.");
    check(productId != nullptr, "product id is missing.");

    // Generate checksum from productId
    checksum256 product_checksum = sha256(productId->c_str(), productId->size());

    // Access table and make sure product exists
    product_table products(get_self(), company->value);
    auto products_bychecksum = products.get_index<"bychecksum"_n>();
    auto product = products_bychecksum.find(product_checksum);
    check(product != products_bychecksum.end(), "product does not exist.");

    // Delete product
    products_bychecksum.erase(product);
}
