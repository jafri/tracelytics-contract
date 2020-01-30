#pragma once

#include <eosio/eosio.hpp>
#include <eosio/privileged.hpp>
#include <eosio/crypto.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp>
#include <eosio/time.hpp>

#include <string>
#include <variant>
#include <type_traits>

using namespace eosio;
using namespace std;

struct InventoryDelta {
  double     quantity;
  std::string site;
  std::string company;
};
struct ProductQuantityBase {
  std::string product;
  double     quantity;
};

struct ProductQuantity: ProductQuantityBase {
  std::map<std::string, std::string> metadata;
};

// #define all_type std::variant< \
//   char, \
//   uint16_t, \
//   int32_t, \
//   uint32_t, \
//   int64_t, \
//   uint64_t, \
//   int128_t, \
//   uint128_t, \
//   signed_int, \
//   unsigned_int, \
//   double, \
//   long double, \
//   time_point, \
//   time_point_sec, \
//   eosio::name, \
//   std::string, \
//   eosio::block_timestamp, \
//   eosio::checksum160, \
//   eosio::checksum256, \
//   eosio::checksum512, \
//   eosio::public_key, \
//   eosio::signature, \
//   eosio::symbol, \
//   eosio::symbol_code, \
//   eosio::asset, \
//   eosio::extended_asset \
// >

namespace ProcessStatus
{
  const std::string PROCESSING = "processing";
  const std::string PROCESSED  = "processed";
  const std::string CANCELLED  = "cancelled";
}
namespace ProcessType
{
  const std::string PROCESS    = "process";
  const std::string SPLIT      = "split";
  const std::string MERGE      = "merge";
  const std::string ADJUSTMENT = "adjustment";
  const std::string SCRAP      = "scrap";
}
namespace ProcessActivity
{
  const std::string START_PROCESS  = "Start Process";
  const std::string EDIT_INPUTS    = "Edit Inputs";
  const std::string EDIT_OUTPUTS   = "Edit Outputs";
  const std::string FINISH_PROCESS = "Finish Process";
}

namespace DeliveryStatus
{
  const std::string CANCELLED = "cancelled";
  const std::string LOADING   = "loading";
  const std::string SHIPPED   = "shipped";
  const std::string DELIVERED = "delivered";
}

namespace DeliveryType
{
  const std::string SEND    = "Send Delivery";
  const std::string RECEIVE = "Receive Delivery";
}
namespace DeliveryActivity
{
  const std::string SEND_DELIVERY    = "Send Delivery";
  const std::string EDIT_CARGO       = "Edit Cargo";
  const std::string RECEIVE_DELIVERY = "Receive Delivery";
  const std::string CANCEL_DELIVERY  = "Cancel Delivery";
}


namespace Actions
{
  const std::string NEW_DELIVERY    = "newdelivery";
  const std::string EDIT_DELIVERY   = "editdelivery";
  const std::string DELETE_DELIVERY = "deldelivery";
  const std::string NEW_ITEM        = "newitem";
  const std::string EDIT_ITEM       = "edititem";
  const std::string DELETE_ITEM     = "delitem";
  const std::string NEW_PROCESS     = "newprocess";
  const std::string EDIT_PROCESS    = "editprocess";
  const std::string DELETE_PROCESS  = "delprocess";
}