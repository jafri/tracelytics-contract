#pragma once

#include <eosio/eosio.hpp>
#include <eosio/privileged.hpp>
#include <eosio/crypto.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp>
#include <eosio/time.hpp>

#include <string>
#include <variant>
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
  int8_t,
  uint8_t,
  int16_t,
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
  const std::string Loading   = "loading";
  const std::string Shipped   = "shipped";
  const std::string Delivered = "delivered";
}

namespace DeliveryType
{
  const std::string Internal = "internal";
  const std::string External = "external";
}

namespace ProcessType
{
  const std::string Process = "process";
  const std::string Split   = "split";
  const std::string Merge   = "merge";
}