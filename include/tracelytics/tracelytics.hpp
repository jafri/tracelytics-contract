#pragma once

#include <eosio/eosio.hpp>
#include <eosio/privileged.hpp>
#include <eosio/crypto.hpp>
#include <eosio/asset.hpp>

#include <string>
#include <variant>
#include <any>

using namespace eosio;
using namespace std;

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
  // vector<std::string>
> all_type;

struct Cargo {
  checksum256 product_checksum;
  uint64_t quantity;
  int64_t delta;
};

CONTRACT tracelytics : public contract {
  public:
    using contract::contract;

    // Create
    ACTION newuser       (std::map<std::string, all_type> args);
    ACTION newbatch      (std::map<std::string, all_type> args);
    ACTION newdelivery   (std::map<std::string, all_type> args, std::map<checksum256, Cargo> cargo);
    ACTION newsite       (std::map<std::string, all_type> args);
    ACTION newmachine    (std::map<std::string, all_type> args);
    ACTION newproduct    (std::map<std::string, all_type> args);
    ACTION newprodins    (std::map<std::string, all_type> args);

    // Patch
    ACTION edituser       (std::map<std::string, all_type> args);
    ACTION editbatch      (std::map<std::string, all_type> args);
    ACTION editdelivery   (std::map<std::string, all_type> args, std::map<checksum256, Cargo> cargo);
    ACTION editsite       (std::map<std::string, all_type> args);
    ACTION editmachine    (std::map<std::string, all_type> args);
    ACTION editproduct    (std::map<std::string, all_type> args);
    ACTION editprodins    (std::map<std::string, all_type> args);

    // Delete
    ACTION deluser       (std::map<std::string, all_type> args);
    ACTION delbatch      (std::map<std::string, all_type> args);
    ACTION deldelivery   (std::map<std::string, all_type> args);
    ACTION delsite       (std::map<std::string, all_type> args);
    ACTION delmachine    (std::map<std::string, all_type> args);
    ACTION delproduct    (std::map<std::string, all_type> args);
    ACTION delprodins    (std::map<std::string, all_type> args);

    ACTION push(
      std::string verifydata,
      const signature &sig,
      const public_key &pk,
      name username,
      name company,
      std::string entity,
      std::string action,
      std::map<std::string, all_type> args, std::map<std::string, all_type> metadata
    );

    // using remove_action = action_wrapper<"remove"_n, &tracelytics::remove>;

  private:
    TABLE User {
      uint64_t id;
      checksum256 user_checksum;
      std::string user_id;
      public_key key;
      uint64_t nonce = 0;
      std::vector<std::string> permissions;

      std::map<std::string, all_type> data;

      uint64_t primary_key() const { return id; };
      checksum256 by_checksum() const { return user_checksum; };

      EOSLIB_SERIALIZE( User, (id)(user_checksum)(user_id)(key)(nonce)(permissions)(data) )
    };

    TABLE Machine {
      uint64_t id;
      checksum256 machine_checksum;
      std::string machine_id;

      std::string name;
      checksum256 site;

      std::map<std::string, all_type> data;
      uint64_t primary_key() const { return id; };
      checksum256 by_checksum() const { return machine_checksum; };
    };
    TABLE Site {
      uint64_t id;
      checksum256 site_checksum;
      std::string site_id;

      std::string name;

      std::map<std::string, all_type> data;
      uint64_t primary_key() const { return id; };
      checksum256 by_checksum() const { return site_checksum; };
    };
    TABLE Delivery {
      uint64_t id;
      checksum256 delivery_checksum;
      std::string delivery_id;

      std::string shipper_id;
      std::string driver_id;
      std::string start_time;
      std::string end_time;
      std::string status;
      std::map<checksum256, Cargo> cargo;

      std::map<std::string, all_type> data;
      uint64_t primary_key() const { return id; };
      checksum256 by_checksum() const { return delivery_checksum; };
    };

    TABLE Batch {
      uint64_t id;
      checksum256 batch_checksum;
      std::string batch_id;

      std::string user_id;
      std::string machine_id;
      std::string date;

      std::map<std::string, all_type> data;

      uint64_t primary_key() const { return id; };
      checksum256 by_checksum() const { return batch_checksum; };
    };
    TABLE Product {
      uint64_t id;
      checksum256 product_checksum;
      std::string product_id;

      std::string name;
      std::string description;

      std::map<std::string, all_type> data;
      uint64_t primary_key() const { return id; };
      checksum256 by_checksum() const {
        return sha256(product_id.c_str(), product_id.size());
      }
    };
    TABLE ProductInstance {
      uint64_t id;
      checksum256 productins_checksum;
      std::string productins_id;

      checksum256 site_checksum;
      std::string site_id;


      std::string product;

      std::map<std::string, all_type> data;
      uint64_t primary_key() const { return id; };
      checksum256 by_checksum() const { return productins_checksum; };
    };

    typedef multi_index<"user"_n, User,
      indexed_by<"bychecksum"_n, const_mem_fun<User, checksum256, &User::by_checksum>>
    > user_table;
    typedef multi_index<"machine"_n, Machine,
      indexed_by<"bychecksum"_n, const_mem_fun<Machine, checksum256, &Machine::by_checksum>>
    > machine_table;
    typedef multi_index<"site"_n, Site,
      indexed_by<"bychecksum"_n, const_mem_fun<Site, checksum256, &Site::by_checksum>>
    > site_table;
    typedef multi_index<"delivery"_n, Delivery,
      indexed_by<"bychecksum"_n, const_mem_fun<Delivery, checksum256, &Delivery::by_checksum>>
    > delivery_table;
    typedef multi_index<"batch"_n, Batch,
      indexed_by<"bychecksum"_n, const_mem_fun<Batch, checksum256, &Batch::by_checksum>>
    > batch_table;
    typedef multi_index<"product"_n, Product,
      indexed_by<"bychecksum"_n, const_mem_fun<Product, checksum256, &Product::by_checksum>>
    > product_table;
    typedef multi_index<"productins"_n, ProductInstance,
      indexed_by<"bychecksum"_n, const_mem_fun<ProductInstance, checksum256, &ProductInstance::by_checksum>>
    > product_instance_table;

    void ec_verify(std::string data, const signature &sig, const public_key &pk);
    void verify_auth(name company, name username, std::string entity, std::string action, std::string verifydata);
    std::vector<std::string> split(std::string str, std::string token);
    std::string to_hex(const char* d, uint32_t s);
};