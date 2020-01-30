#pragma once

/**
 * inventoryDelta has 3 states
 * Zero: No change
 * Positive (+): +Inventory <- -Cargo (refund site)
 * Negative (-): -Inventory -> +Cargo (charge site)
 **/

/**
 * Processes
**/
InventoryDelta tracelytics::inventoryDeltaForCargo (
  const Process& entity,
  const ProductQuantity& productAndQuantity,
  const std::string& activity
) {
  double inventoryDelta = 0;

  // Start process -> CHARGE site
  if (activity == ProcessActivity::START_PROCESS) {
    inventoryDelta = -productAndQuantity.quantity;
  // Finish process or merge or split -> CREDIT Site
  } else if (activity == ProcessActivity::FINISH_PROCESS) {
    inventoryDelta = productAndQuantity.quantity;
  } else {
    check(false, "not of type start process, finish process, split or merge");
  }

  return InventoryDelta {
    inventoryDelta,
    entity.site,
    entity.company
  };
}

/**
 * End Processes
**/

void tracelytics::processcargo (
  Process& entity,
  std::map<std::string, ProductQuantity>& cargo,
  const std::map<std::string, ProductQuantity>& cargoDeltas,
  const std::string& user,
  const std::string& company,
  const std::string& action,
  const std::string& activity
) {
  bool isDeltas = cargoDeltas.size() > 0;
  auto deltasOrCargo = isDeltas ? cargoDeltas : cargo;

  for( auto const& [item, productAndQuantity] : deltasOrCargo ) {
    InventoryDelta inventoryDelta;

    // 1. Validate quantity
    check(productAndQuantity.quantity >= 0, "quantity in delivery must be zero or positive for item " + item);

    // 2. Get inventory delta
    if (isDeltas) {
      auto inventoryDeltaQuantity = cargo.count(item) > 0
        ? cargo[item].quantity - productAndQuantity.quantity
        : -productAndQuantity.quantity;

      inventoryDelta = InventoryDelta {
        inventoryDeltaQuantity,
        entity.site,
        entity.company
      };
    } else {
      inventoryDelta = inventoryDeltaForCargo(entity, productAndQuantity, activity);
    }

    // 3. Update cargo with new quantities if processing deltas
    if (isDeltas) {
      // 3a. Erase if 0
      if (productAndQuantity.quantity == 0) {
        cargo.erase(item);
      // 3b. Set if positive
      } else {
        cargo[item] = productAndQuantity;
      }
    }

    // 4. Update item quantities
    // Skip if delta is 0 and if not editing outputs for a process
    if (inventoryDelta.quantity != 0 && activity != ProcessActivity::EDIT_OUTPUTS) {
      upsertitem(user,
                 inventoryDelta.company,
                 inventoryDelta.site,
                 item,
                 productAndQuantity.product,
                 inventoryDelta.quantity,
                 productAndQuantity.metadata,
                 action,
                 entity.id(),
                 entity.updatedAt);
    }
  }
};

// Deltas quantity if exists
// Create item with delta as quantity if does not exist
// Make sure delta is positive if item doesnt exist
void tracelytics::upsertitem (
    const std::string& user,
    const std::string& company,
    const std::string& site,
    const std::string& item,
    const std::string& product,
    const double& delta,
    std::map<std::string, std::string> metadata,
    const std::string& action,
    const std::string& actionId,
    const time_point& timestamp
) {
    // Existing item
    auto items_byid = _items.get_index<eosio::name("byid")>();
    auto existing_item = items_byid.find(Checksum::ITEM(item));

    // Empty call data
    std::map<std::string, std::string> call_data;

    // Item exists
    if (existing_item != items_byid.end()) {
        // Make sure the site matches
        check(existing_item->site == site, "item " + item + " is now at " + existing_item->site + ", cannot remove from process.");

        // Make sure the site has enough items after delta
        check(existing_item->quantity + delta >= 0, "Site " + site + " has " +
                                                    to_string(existing_item->quantity) + " " + item +
                                                    "(" + product + "), you are trying to use " + to_string(-delta));

        tracelytics::edititem_action edititem_action( get_self(), {get_self(), eosio::name("active")} );
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
                             std::nullopt,
                             std::nullopt);
    // Item does not exist (create it)
    } else {
        // Make sure we are not trying to create with negative delta
        check(delta > 0, "Item " + item + " does not exist at site " + site + ". Please create it first.");

        tracelytics::newitem_action newitem_action( get_self(), {get_self(), eosio::name("active")} );
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
