#include "tracelytics/tracelytics.hpp"

/**
 * Create New Product
 **/
void tracelytics::newproduct (
    const std::string& user,
    const std::string& company,
    const std::string& productId,
    const std::string& uom,
    const time_point& timestamp,
    const std::map<std::string, all_type>& data,

    const optional<std::string>& name,
    const optional<std::string>& image,
    const optional<std::string>& description,
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),      "user is missing.");
    check(!company.empty(),   "company is missing.");
    check(!productId.empty(), "product id is missing.");
    check(!uom.empty(),       "unit of measure is missing.");

    // Generate checksum from productId
    std::string product_checksum_string = company + ";" + productId;
    checksum256 product_checksum = sha256(product_checksum_string.c_str(), product_checksum_string.size());

    // Access table and make sure product doesnt exist
    product_table products(get_self(), get_self().value);
    auto products_bychecksum = products.get_index<"bychecksum"_n>();
    auto product = products_bychecksum.find(product_checksum);
    check(product == products_bychecksum.end(), "product already exists");

    // Create new product
    products.emplace(get_self(), [&](auto& p) {
        p.index     = products.available_primary_key();
        p.createdBy = user;
        p.updatedBy = user;
        p.createdAt = timestamp;
        p.updatedAt = timestamp;

        p.company   = company;
        p.productId = productId;
        p.uom       = uom;

        // Optional
        if (name)        p.name        = *name;
        if (image)       p.image       = *image;
        if (description) p.description = *description;
        if (version)     p.version     = *version;
    });
}

/**
 * Edit Product
 **/
void tracelytics::editproduct (
    const std::string& user,
    const std::string& company,
    const std::string& productId,
    const time_point& timestamp,
    const std::map<std::string, all_type>& data,

    const optional<std::string>& uom,
    const optional<std::string>& name,
    const optional<std::string>& image,
    const optional<std::string>& description,
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),      "user is missing.");
    check(!company.empty(),   "company is missing.");
    check(!productId.empty(), "product id is missing.");

    // Generate checksum from productId
    std::string product_checksum_string = company + ";" + productId;
    checksum256 product_checksum = sha256(product_checksum_string.c_str(), product_checksum_string.size());

    // Access table and make sure product exists
    product_table products(get_self(), get_self().value);
    auto products_bychecksum = products.get_index<"bychecksum"_n>();
    auto product = products_bychecksum.find( product_checksum );
    check(product != products_bychecksum.end(), "product does not exist.");

    // Edit Product
    products_bychecksum.modify(product, get_self(), [&](auto& p) {
        p.updatedBy = user;
        p.updatedAt = timestamp;

        // Optional
        if (name)        p.name        = *name;
        if (uom)         p.uom         = *uom;
        if (image)       p.image       = *image;
        if (description) p.description = *description;
        if (version)     p.version     = *version;
    });
}

/**
 * Delete Product
 **/
void tracelytics::delproduct (
    const std::string& user,
    const std::string& company,
    const std::string& productId,
    const time_point&  timestamp
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),      "user is missing.");
    check(!company.empty(),   "company is missing.");
    check(!productId.empty(), "product id is missing.");

    // Generate checksum from productId
    std::string product_checksum_string = company + ";" + productId;
    checksum256 product_checksum = sha256(product_checksum_string.c_str(), product_checksum_string.size());

    // Access table and make sure product exists
    product_table products(get_self(), get_self().value);
    auto products_bychecksum = products.get_index<"bychecksum"_n>();
    auto product = products_bychecksum.find(product_checksum);
    check(product != products_bychecksum.end(), "product does not exist.");

    // Delete product
    products_bychecksum.erase(product);
}
