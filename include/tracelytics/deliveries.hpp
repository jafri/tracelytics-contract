#pragma once

template <typename T>
void tracelytics::processcargo (
  T& entity,
  std::map<std::string, ProductQuantity>& cargo,
  const std::map<std::string, ProductQuantity>& cargoDeltas,
  const std::string& user,
  const std::string& company,
  const std::string& action,
  const std::string& type,
  const bool& skipUpsert
) {
  auto mapOfItems = cargoDeltas.size() > 0 ? cargoDeltas : cargo;

  for( auto const& [item, productAndQuantity] : mapOfItems ) {
    int64_t inventoryDelta = 0;
    std::string inventoryDeltaSite;

    // New Delivery/Process (No deltas)
    if (cargoDeltas.size() == 0) {
      if constexpr (std::is_same_v<T, Delivery>) {
        // Send delivery: Raptor -> Supplier
        if (type == DeliveryActions::SEND_DELIVERY) {
          inventoryDelta = -productAndQuantity.quantity;
          inventoryDeltaSite = entity.fromSite;
        // Receive delivery: Supplier -> Raptor
        } else if (type == DeliveryActions::RECEIVE_DELIVERY) {
          inventoryDelta = productAndQuantity.quantity;
          inventoryDeltaSite = entity.toSite;
        } else if (type == DeliveryActions::CANCEL_DELIVERY) {
          inventoryDelta = productAndQuantity.quantity;
          inventoryDeltaSite = entity.fromSite;
        } else {
          check(false, "not of type send delivery, receive delivery or receive magic delivery.");
        }
      } else if constexpr (std::is_same_v<T, Process>) {
        inventoryDeltaSite = entity.site;

        // Start process -> CHARGE site
        if (type == ProcessActions::START_PROCESS) {
          inventoryDelta = -productAndQuantity.quantity;
        // Finish process or merge or split -> CREDIT Site
        } else if (type == ProcessActions::FINISH_PROCESS) {
          inventoryDelta = productAndQuantity.quantity;
        } else {
          check(false, "not of type start process, finish process, split or merge");
        }
      } else {
        check(false, "only delivery and process are allowed");
      }
    // Edit Delivery/Process (deltas)
    } else {
      /**
       * inventoryDelta has 3 states
       * Zero: No change
       * Positive (+): +Inventory <- -Cargo (refund site)
       * Negative (-): -Inventory -> +Cargo (charge site)
       **/

      // ALWAYS from site, as that is the one we refund to
      if constexpr (std::is_same_v<T, Delivery>) {
        inventoryDeltaSite = entity.fromSite;
      } else if constexpr (std::is_same_v<T, Process>) {
        inventoryDeltaSite = entity.site;
      } else {
        check(false, "only delivery and process are allowed");
      }

      // In cargo = POSITIVE/NEGATIVE DELTA
      if (cargo.count(item) > 0) {
        inventoryDelta = cargo[item].quantity - productAndQuantity.quantity;
      // Not in cargo = NEGATIVE DELTA
      } else {
        inventoryDelta = -productAndQuantity.quantity;
      }

      // Update cargo with new quantity
      if (productAndQuantity.quantity > 0) {
        cargo[item] = productAndQuantity;
      } else if (productAndQuantity.quantity == 0) {
        cargo.erase(item);
      } else {
        // Ensure new quantity is zero or positive
        check(false, "quantity in delivery must be zero or positive for item " + item);
      }
    }

    // Skip if delta is 0, as nothing changes
    if (!skipUpsert && inventoryDelta != 0) {
      upsertitem(user,
                company,
                inventoryDeltaSite,
                item,
                productAndQuantity.product,
                inventoryDelta,
                productAndQuantity.metadata,
                action,
                entity.id(),
                entity.updatedAt);
    }
  }
};