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
#include <any>

using namespace eosio;
using namespace std;

struct ProductQuantityBase {
  std::string product;
  int64_t     quantity;
};

struct ProductQuantity: ProductQuantityBase {
  std::map<std::string, std::string> metadata;
};

typedef std::variant<
  char,
  bool,
  uint64_t,
  uint16_t,
  int32_t,
  uint32_t,
  int64_t,
  uint64_t,
  int128_t,
  uint128_t,
  signed_int,
  unsigned_int,
  float,
  double,
  long double,
  time_point,
  time_point_sec,
  name,
  std::string,
  block_timestamp,
  checksum160,
  checksum256,
  checksum512,
  public_key,
  signature,
  symbol,
  symbol_code,
  asset,
  extended_asset
> all_type;

typedef std::variant<
  int64_t,
  uint64_t,
  name,
  std::string,
  block_timestamp,
  checksum256,
  public_key,
  signature,
  extended_asset
> all_basic_type;

namespace DeliveryStatus
{
  const std::string LOADING   = "loading";
  const std::string SHIPPED   = "shipped";
  const std::string DELIVERED = "delivered";
}

namespace DeliveryType
{
  const std::string RECEIVE = "receive";
  const std::string SEND    = "send";
}

namespace ProcessType
{
  const std::string PROCESS = "process";
  const std::string SPLIT   = "split";
  const std::string MERGE   = "merge";
}

namespace ProcessStatus
{
  const std::string PROCESSED = "processed";
}

namespace CargoMovementType
{
  const std::string CHARGE = "charge";
  const std::string CREDIT = "credit";
}

namespace Tables
{
  const std::string DELIVERY = "delivery";
  const std::string ITEM = "item";
  const std::string PROCESS = "process";
}

namespace Actions
{
  const std::string NEW_DELIVERY    = "newdelivery";
  const std::string EDIT_DELIVERY    = "editdelivery";
  const std::string NEW_ITEM         = "newitem";
  const std::string EDIT_ITEM        = "edititem";
  const std::string DELETE_ITEM      = "delitem";
  const std::string NEW_PROCESS      = "newprocess";
  const std::string EDIT_PROCESS     = "editprocess";
  const std::string DELETE_PROCESS   = "delprocess";
}