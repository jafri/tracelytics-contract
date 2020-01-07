#pragma once

template <typename T>
void tracelytics::processcargo (
  T& entity,
  std::map<std::string, ProductQuantity>& cargo,
  const std::map<std::string, ProductQuantity>& cargoDeltas,
  const std::string& user,
  const std::string& company,
  const std::string& action,
  const bool& skipUpsert
) {
  bool editCargo = cargoDeltas.size() > 0;

  for( auto const& [item, productAndQuantity] : editCargo ? cargoDeltas : cargo ) {
    int64_t inventoryDelta;
    std::string inventoryDeltaSite;

    // New Delivery/Process
    if (!editCargo) {
        // New Delivery
        if constexpr (std::is_same_v<T, Delivery>) {
            // Raptor -> Supplier
            // Negative delta = CHARGE SITE
            if (company == entity.fromCompany) {
                inventoryDelta = -productAndQuantity.quantity;
                inventoryDeltaSite = entity.fromSite;
            // Supplier -> Raptor
            // Positive delta = CREDIT SITE
            // ~~~~~~~ MAGIC MOCK DELIVERY ~~~~~~~~~
            // Just credits the receiving site without charging the sender
            } else if (company == entity.toCompany) {
                inventoryDelta = productAndQuantity.quantity;
                inventoryDeltaSite = entity.toSite;
            } else {
                check(false, "user must be from sending or receiving company");
            }
        } else if constexpr (std::is_same_v<T, Process>) {
            // Finish process, merge or split -> CREDIT Site
            if (entity.status == ProcessStatus::PROCESSED) {
              inventoryDelta = productAndQuantity.quantity;
            // Start process -> CHARGE site
            } else {
              inventoryDelta = -productAndQuantity.quantity;
            }
            inventoryDeltaSite = entity.site;
        } else {
            check(false, "only delivery and process are allowed");
        }
    // Edit Delivery/Process
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

      // Ensure new quantity is zero or positive
      check(productAndQuantity.quantity >= 0, "quantity in delivery must be zero or positive for item " + item);

      // Update cargo
      if (productAndQuantity.quantity > 0) {
          cargo[item] = productAndQuantity;
      } else if (productAndQuantity.quantity == 0) {
          cargo.erase(item);
      }

      // In cargo = POSITIVE/NEGATIVE DELTA
      if (cargo.count(item) > 0) {
          inventoryDelta = cargo[item].quantity - productAndQuantity.quantity;
      // Not in cargo = NEGATIVE DELTA
      } else {
          inventoryDelta = -productAndQuantity.quantity;
      }
    }

    // Skip if delta is 0, as nothing changes
    if (inventoryDelta == 0 || skipUpsert) {
        continue;
    }

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
};