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
    const int64_t& quantity,
    std::map<std::string, std::string> metadata,

    const std::string& action,
    const std::string& actionId,
    const time_point& timestamp,
    const std::map<std::string, all_type>& data,

    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),    "user is missing.");
    check(!company.empty(), "company is missing.");
    check(!itemId.empty(),  "item id is missing.");
    check(!site.empty(),    "site is missing.");
    check(!product.empty(), "product is missing.");
    check(quantity > 0,     "quantity must be positive to create item.");

    // Generate checksum from itemId
    std::string item_checksum_string = company + ";" + site + ";" + itemId;
    checksum256 item_checksum = sha256(item_checksum_string.c_str(), item_checksum_string.size());

    // Access table and make sure item doesnt exist
    item_table items(get_self(), get_self().value);
    auto item_bychecksum = items.get_index<"bysiteitem"_n>();
    auto item = item_bychecksum.find(item_checksum);
    check(item == item_bychecksum.end(), "Error creating item " + itemId + " as it already exists at site " + site);

    // Create new item
    items.emplace(get_self(), [&](auto& p) {
        p.index     = items.available_primary_key();
        p.createdBy  = user;
        p.updatedBy  = user;
        p.createdAt  = timestamp;
        p.updatedAt  = timestamp;

        p.company   = company;
        p.site      = site;
        p.itemId    = itemId;
        p.quantity  = quantity;
        p.product   = product;

        // Optional
        if (metadata.count("parent"))      p.metadata["parent"]      = metadata["parent"];
        if (metadata.count("description")) p.metadata["description"] = metadata["description"];
        if (metadata.count("image"))       p.metadata["image"]       = metadata["image"];

        // Log the new quantity
        tracelytics::loginventory_action loginventory_action( get_self(), {get_self(), "active"_n} );
        loginventory_action.send(p.createdBy,
                                 p.company,
                                 p.itemId,
                                 p.site,
                                 p.product,
                                 p.metadata,
                                 (std::string) "newitem",
                                 action,
                                 actionId,
                                 p.createdAt,
                                 p.version,
                                 (int64_t) 0,
                                 p.quantity);
    });
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
    const std::map<std::string, all_type>& data,

    const optional<int64_t>& quantity,
    const optional<int64_t>& delta,
    const optional<std::string>& product,
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),    "user is missing.");
    check(!company.empty(), "company is missing.");
    check(!itemId.empty(),  "item id is missing.");
    check(!site.empty(),    "site is missing.");
    check(quantity || delta, "both quantity and delta are missing."); // Only provide 1 of quantity or delta

    // Generate checksum from itemId
    std::string item_checksum_string = company + ";" + site + ";" + itemId;
    checksum256 item_checksum = sha256(item_checksum_string.c_str(), item_checksum_string.size());

    // Access table and make sure item exists
    item_table items(get_self(), get_self().value);
    auto item_bychecksum = items.get_index<"bysiteitem"_n>();
    auto item = item_bychecksum.find( item_checksum );
    check(item != item_bychecksum.end(), "Error editing item " + itemId + " as it does not exist at site " + site);

    // Edit item
    item_bychecksum.modify(item, get_self(), [&](auto& p) {
        p.updatedBy = user;
        p.updatedAt = timestamp;

        // Optional
        if (product) p.product = *product;
        if (version) p.version = *version;
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
        item_bychecksum.erase(item);
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
    check(!itemId.empty(),  "item id is missing.");
    check(!site.empty(),    "site is missing.");

    // Generate checksum from itemId
    std::string item_checksum_string = company + ";" + site + ";" + itemId;
    checksum256 item_checksum = sha256(item_checksum_string.c_str(), item_checksum_string.size());

    // Access table and make sure item exists
    item_table items(get_self(), get_self().value);
    auto item_bychecksum = items.get_index<"bysiteitem"_n>();
    auto item = item_bychecksum.find(item_checksum);
    check(item != item_bychecksum.end(), "Error deleting item " + itemId + " as it does not exist at site " + site);

    // Log quantity change
    tracelytics::loginventory_action loginventory_action( get_self(), {get_self(), "active"_n} );
    loginventory_action.send(item->updatedBy,
                             item->company,
                             item->itemId,
                             item->site,
                             item->product,
                             item->metadata,
                             (std::string) "delitem",
                             action,
                             actionId,
                             timestamp,
                             item->version,
                             item->quantity,
                             (int64_t) 0);

    // Delete item
    item_bychecksum.erase(item);
}

// Deltas quantity if exists
// Create item with delta as quantity if does not exist
// Make sure delta is positive if item doesnt exist
void tracelytics::upsertitem (
    const std::string& user,
    const std::string& company,
    const std::string& site,
    const std::string& item,
    const std::string& product,
    const int64_t& delta,
    std::map<std::string, std::string> metadata,
    const std::string& action,
    const std::string& actionId,
    const time_point& timestamp
) {
    // Checksum
    std::string item_checksum_string = company + ";" + site + ";" + item;
    checksum256 item_checksum = sha256(item_checksum_string.c_str(), item_checksum_string.size());

    // Existing item
    item_table items(get_self(), get_self().value);
    auto items_bychecksum = items.get_index<"bysiteitem"_n>();
    auto existing_item = items_bychecksum.find( item_checksum );

    // Empty call data
    std::map<std::string, all_type> call_data;

    // Item exists at site
    if (existing_item != items_bychecksum.end()) {
        // Make sure the site has enough items after delta
        check(existing_item->quantity + delta >= 0, "Site " + site + " has " +
                                                    to_string(existing_item->quantity) + " " + item +
                                                    "(" + product + "), you are trying to use " + to_string(-delta));

        tracelytics::edititem_action edititem_action( get_self(), {get_self(), "active"_n} );
        edititem_action.send(user,
                             company,
                             site,
                             item,
                             metadata,
                             action,
                             actionId,
                             timestamp,
                             call_data,
                             std::nullopt,
                             delta,
                             product,
                             std::nullopt);
    // Item does not exist at site
    } else {
        // Make sure we are not trying to create with negative delta
        check(delta > 0, "Item " + item + " does not exist at site " + site + ". Please create it first.");

        tracelytics::newitem_action newitem_action( get_self(), {get_self(), "active"_n} );
        newitem_action.send( user,
                             company,
                             site,
                             item,
                             product,
                             delta,
                             metadata,
                             action,
                             actionId,
                             timestamp,
                             call_data,
                             std::nullopt);
    }
}
