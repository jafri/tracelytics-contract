#pragma once

void tracelytics::processDelivery (
  Delivery& entity,
  std::map<std::string, ProductQuantity>& cargo,
  const std::string& activity,
  const std::string& deliveryAction
) {
  // Item index
  auto items_byid = _items.get_index<eosio::name("byid")>();

  for( auto const& [item, productAndQuantity] : cargo ) {
    // 1. Edit the cargo
    bool addCargo = activity == DeliveryActivity::EDIT_CARGO && productAndQuantity.quantity > 0;
    bool removeCargo = activity == DeliveryActivity::EDIT_CARGO && productAndQuantity.quantity == 0;
    if (addCargo) {
      entity.cargo[item] = productAndQuantity;
    } else if (removeCargo) {
      entity.cargo.erase(item);
    }

    // 2. Validate item
    // 2.1 Check item exists
    auto existing_item = items_byid.find(Checksum::ITEM(item));
    check(existing_item != items_byid.end(), "item " + item + " does not exist");
    // 2.2 Items have a positive quantity
    check(existing_item->quantity > 0, "item " + item + " has a quantity of " + to_string(existing_item->quantity) + ", must be positive to transfer.");
    // 2.3 Check item quantity is transferring ALL or removing cargo
    check(existing_item->quantity == productAndQuantity.quantity || removeCargo, "Trying to transfer " + to_string(productAndQuantity.quantity) + " of " + item + ", but must send " + to_string(existing_item->quantity) + ". Please split items into 2 tags if you wish to send partial quantity.");

    // 3. Modify item to reflect state
    items_byid.modify(existing_item, same_payer, [&](auto& i){
      i.updatedAt = entity.updatedAt;
      i.updatedBy = entity.updatedBy;

      // 3a. Sending delivery/ Add to cargo: Move to delivery mode
      if (activity == DeliveryActivity::SEND_DELIVERY || addCargo) {
        i.company  = entity.fromCompany + " -> " + entity.toCompany;
        i.site     = entity.fromSite + " -> " + entity.toSite;
        i.delivery = entity.deliveryId;

      // 3b. Receive delivery: Credit to receiver
      } else if (activity == DeliveryActivity::RECEIVE_DELIVERY) {
        i.company  = entity.toCompany;
        i.site     = entity.toSite;
        i.delivery = (std::string) "";

      // 3c. Cancel delivery/Remove cargo: Credit to sender
      } else if (activity == DeliveryActivity::CANCEL_DELIVERY || removeCargo) {
        i.company  = entity.fromCompany;
        i.site     = entity.fromSite;
        i.delivery = (std::string) "";

      // 3d. Error
      } else {
        check(false, "not of type send delivery, receive delivery or cancel delivery.");
      }

      // Log the change
      tracelytics::loginventory_action loginventory_action( get_self(), {get_self(), "active"_n} );
      loginventory_action.send(i.updatedBy,
                               i.company,
                               i.itemId,
                               i.site,
                               i.product,
                               i.delivery,
                               i.metadata,
                               (std::string) "edititem",
                               deliveryAction,
                               entity.deliveryId,
                               entity.updatedAt,
                               i.version,
                               i.quantity,
                               i.quantity);
    });
  }
};