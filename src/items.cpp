#include "tracelytics/tracelytics.hpp"

/**
 * Create New item
 **/
void tracelytics::newitem (
    const std::string& user,
    const std::string& company,
    const std::string& site,
    const std::string& itemId,
    const std::string& product,
    const double& quantity,
    std::map<std::string, std::string> metadata,

    const std::string& action,
    const std::string& actionId,
    const time_point& timestamp,
    const std::map<std::string, std::string>& data,

    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),    "user is missing.");
    check(!company.empty(), "company is missing.");
    check(!itemId.empty(),  "item ID is missing.");
    check(!site.empty(),    "site is missing.");
    check(!product.empty(), "product is missing.");
    check(quantity > 0,     "quantity must be positive to create item.");

    // Check sites exist
    check_site_exists(company, site);

    // Access table and make sure item doesnt exist
    auto items_byid = _items.get_index<eosio::name("byid")>();
    auto item = items_byid.find(Checksum::ITEM(itemId));
    check(item == items_byid.end(), "Error creating item " + itemId + " as it already exists at site " + item->site);

    // Create new item
    auto new_item = _items.emplace(get_self(), [&](auto& p) {
        p.index     = _items.available_primary_key();
        p.createdBy = user;
        p.updatedBy = user;
        p.createdAt = timestamp;
        p.updatedAt = timestamp;

        p.company   = company;
        p.site      = site;
        p.itemId    = itemId;
        p.quantity  = quantity;
        p.product   = product;

        // Optional
        if (metadata.count("parent"))      p.metadata["parent"]      = metadata["parent"];
        if (metadata.count("description")) p.metadata["description"] = metadata["description"];
        if (metadata.count("image"))       p.metadata["image"]       = metadata["image"];
    });

    // Log the new quantity
    tracelytics::loginventory_action loginventory_action( get_self(), {get_self(), "active"_n} );
    loginventory_action.send(new_item->createdBy,
                             new_item->company,
                             new_item->itemId,
                             new_item->site,
                             new_item->product,
                             new_item->delivery,
                             new_item->metadata,
                             (std::string) "newitem",
                             action,
                             actionId,
                             new_item->createdAt,
                             new_item->version,
                             0.0,
                             new_item->quantity);
}

/**
 * Edit item
 **/
void tracelytics::edititem (
    const std::string& user,
    const std::string& company,
    const std::string& site,
    const std::string& itemId,
    std::map<std::string, std::string> metadata,
    const std::string& action,
    const std::string& actionId,
    const time_point&  timestamp,
    const std::map<std::string, std::string>& data,
    const optional<double>& quantity,
    const optional<double>& delta,
    const optional<std::string>& product,
    const optional<std::string>& delivery,
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),    "user is missing.");
    check(!company.empty(), "company is missing.");
    check(!itemId.empty(),  "item ID is missing.");
    check(!site.empty(),    "site is missing.");
    check(quantity || delta, "both quantity and delta are missing. Must provide one."); // Only provide 1 of quantity or delta

    // Access table and make sure item exists
    auto items_byid = _items.get_index<eosio::name("byid")>();
    auto item = items_byid.find(Checksum::ITEM(itemId));
    check(item != items_byid.end(), "Error editing item " + itemId + " as it does not exist at site " + site);
    check(itemId == item->itemId, "item mismatch");

    // Ensure editing user is from company
    if (user != ADMIN) {
        check(company == item->company, "only employees of " + item->company + " can edit item " + itemId + ". Current user is from" + company + ".");
    }

    // Edit item
    items_byid.modify(item, get_self(), [&](auto& p) {
        p.updatedBy = user;
        p.updatedAt = timestamp;

        // Optional
        if (product)  p.product  = *product;
        if (delivery) p.delivery = *delivery;
        if (version)  p.version  = *version;
        if (metadata.count("parent"))      p.metadata["parent"]      = metadata["parent"];
        if (metadata.count("description")) p.metadata["description"] = metadata["description"];

        // Update quantity and log it
        // MAKE SURE this is at the end of the modify function, so it uses updated product/site values
        if (quantity || delta) {
            auto oldQuantity = p.quantity;

            // If quantity, set quantity to equal new quantity
            // If delta, change quantity by delta quantity
            if (quantity) {
                p.quantity = *quantity;
            } else if (delta) {
                p.quantity += *delta;
            }

            // Validation (must be 0 or positive)
            check(p.quantity >= 0, "Item " + p.itemId + " quantity at site " + p.site + " must be zero or positive. Provided quantity: " + to_string(p.quantity));

            // Log quantity change
            tracelytics::loginventory_action loginventory_action( get_self(), {get_self(), "active"_n} );
            loginventory_action.send(p.updatedBy,
                                     p.company,
                                     p.itemId,
                                     p.site,
                                     p.product,
                                     p.delivery,
                                     p.metadata,
                                     (std::string) "edititem",
                                     action,
                                     actionId,
                                     p.updatedAt,
                                     p.version,
                                     oldQuantity,
                                     p.quantity);
        }
    });

    // Delete if quantity is 0
    if (item->quantity == 0) {
        items_byid.erase(item);
    }
}

/**
 * Delete item
 **/
void tracelytics::delitem (
    const std::string& user,
    const std::string& company,
    const std::string& site,
    const std::string& itemId,
    const std::string& action,
    const std::string& actionId,
    const time_point&  timestamp
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),    "user is missing.");
    check(!company.empty(), "company is missing.");
    check(!itemId.empty(),  "item ID is missing.");
    check(!site.empty(),    "site is missing.");

    // Access table and make sure item exists
    auto items_byid = _items.get_index<eosio::name("byid")>();
    auto item = items_byid.find(Checksum::ITEM(itemId));
    check(item != items_byid.end(), "Error deleting item " + itemId + " as it does not exist at site " + site);
    check(site == item->site && itemId == item->itemId, "item mismatch");

    if (user != ADMIN) {
        check(company == item->company, "only employees of " + item->company + " can delete the item.");
    }

    // Log quantity change
    tracelytics::loginventory_action loginventory_action( get_self(), {get_self(), "active"_n} );
    loginventory_action.send(item->updatedBy,
                             item->company,
                             item->itemId,
                             item->site,
                             item->product,
                             item->delivery,
                             item->metadata,
                             (std::string) "delitem",
                             action,
                             actionId,
                             timestamp,
                             item->version,
                             item->quantity,
                             0.0);

    // Delete item
    items_byid.erase(item);
}