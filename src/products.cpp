#include "tracelytics/tracelytics.hpp"

/**
 * Create New Product
 **/
void tracelytics::newproduct (
    const std::string& user,
    const std::string& company,
    const std::string& productId,
    const std::string& name,
    const std::string& uom,
    const int64_t& defaultPrice,
    const std::string& defaultCurrency,
    const time_point& timestamp,
    const std::map<std::string, all_type>& data,

    const optional<std::string>& image,
    const optional<std::string>& description,
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),            "user is missing.");
    check(!company.empty(),         "company is missing.");
    check(!productId.empty(),       "product id is missing.");
    check(!name.empty(),            "name is missing.");
    check(!uom.empty(),             "unit of measure is missing.");
    check(defaultPrice >= 0,        "default price cannot be negative.");
    check(!defaultCurrency.empty(), "default currency is missing.");

    // Generate checksum from productId
    std::string product_checksum_string = productId;
    checksum256 product_checksum = sha256(product_checksum_string.c_str(), product_checksum_string.size());

    // Access table and make sure product doesnt exist
    auto products_byid = _products.get_index<eosio::name("byid")>();
    auto product = products_byid.find(product_checksum);
    check(product == products_byid.end(), "product already exists");

    // Create new product
    _products.emplace(get_self(), [&](auto& p) {
        p.index     = _products.available_primary_key();
        p.createdBy = user;
        p.updatedBy = user;
        p.createdAt = timestamp;
        p.updatedAt = timestamp;

        p.productId       = productId;
        p.name            = name;
        p.uom             = uom;
        p.defaultPrice    = defaultPrice;
        p.defaultCurrency = defaultCurrency;

        // Optional
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

    const optional<std::string>& name,
    const optional<std::string>& uom,
    const optional<int64_t>& defaultPrice,
    const optional<std::string>& defaultCurrency,
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
    std::string product_checksum_string = productId;
    checksum256 product_checksum = sha256(product_checksum_string.c_str(), product_checksum_string.size());

    // Access table and make sure product exists
    auto products_byid = _products.get_index<eosio::name("byid")>();
    auto product = products_byid.find( product_checksum );
    check(product != products_byid.end(), "product does not exist.");
    check(productId == product->productId, "product mismatch");

    // Edit Product
    products_byid.modify(product, get_self(), [&](auto& p) {
        p.updatedBy = user;
        p.updatedAt = timestamp;

        // Optional
        if (name)                              p.name            = *name;
        if (uom)                               p.uom             = *uom;
        if (defaultPrice && *defaultPrice > 0) p.defaultPrice    = *defaultPrice;
        if (defaultCurrency)                   p.defaultCurrency = *defaultCurrency;
        if (image)                             p.image           = *image;
        if (description)                       p.description     = *description;
        if (version)                           p.version         = *version;
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
    std::string product_checksum_string = productId;
    checksum256 product_checksum = sha256(product_checksum_string.c_str(), product_checksum_string.size());

    // Access table and make sure product exists
    auto products_byid = _products.get_index<eosio::name("byid")>();
    auto product = products_byid.find(product_checksum);
    check(product != products_byid.end(), "product does not exist.");
    check(productId == product->productId, "product mismatch");

    // Delete product
    products_byid.erase(product);
}
