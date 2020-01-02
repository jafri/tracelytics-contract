#pragma once

#include <tracelytics/types.hpp>

CONTRACT tracelytics : public contract {
  public:
    using contract::contract;

    // Create
    ACTION newdelivery  ( const std::string& user,
                          const std::string& company,
                          const std::string& deliveryId,
                          const std::string& type,
                          const std::string& fromSite,
                          const std::string& toSite,
                          const time_point& startTime,
                          const std::map<std::string, ProductQuantity>& cargo,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,
                          const optional<time_point>& endTime,
                          const optional<std::string>& shipper,
                          const optional<std::string>& driver,
                          const optional<std::string>& status,
                          const optional<std::string>& description,
                          const optional<std::string>& version );
    ACTION newitem      ( const std::string& user,
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
                          const optional<std::string>& version);
    ACTION newmachine   ( const std::string& user,
                          const std::string& company,
                          const std::string& machineId,
                          const std::string& site,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,
                          const optional<std::string>& name,
                          const optional<std::string>& description,
                          const optional<std::string>& version);
    ACTION newprocess   ( const std::string& user,
                          const std::string& company,
                          const std::string& processId,
                          const std::string& type,
                          const std::string& site,
                          const time_point& startTime,
                          const std::map<std::string, ProductQuantity>& inputs,
                          const std::map<std::string, ProductQuantity>& outputs,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,
                          const optional<time_point>& endTime,
                          const optional<std::string>& machine,
                          const optional<std::string>& status,
                          const optional<std::string>& description,
                          const optional<std::string>& version);
    ACTION newproduct   ( const std::string& user,
                          const std::string& company,
                          const std::string& productId,
                          const std::string& uom,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,
                          const optional<std::string>& name,
                          const optional<std::string>& image,
                          const optional<std::string>& description,
                          const optional<std::string>& version);
    ACTION newrecipe    ( const std::string& user,
                          const std::string& company,
                          const std::string& recipeId,
                          const std::vector<ProductQuantity>& inputs,
                          const std::vector<ProductQuantity>& outputs,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,
                          const optional<std::string>& name,
                          const optional<std::string>& description,
                          const optional<std::string>& version);
    ACTION newsite      ( const std::string& user,
                          const std::string& company,
                          const std::string& siteId,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,
                          const optional<std::string>& name,
                          const optional<std::string>& description,
                          const optional<std::string>& version);
    ACTION newuser      ( const std::string& user,
                          const std::string& company,
                          const std::string& userId,
                          const std::vector<std::string>& permissions,
                          const std::vector<std::string>& certifications,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,
                          const optional<public_key>& key,
                          const optional<std::string>& firstName,
                          const optional<std::string>& lastName,
                          const optional<std::string>& email,
                          const optional<std::string>& description,
                          const optional<std::string>& version);

    // Patch
    ACTION editdelivery ( const std::string& user,
                          const std::string& company,
                          const std::string& deliveryId,
                          const std::map<std::string, ProductQuantity>& cargoDelta,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,
                          const optional<std::string>& toSite,
                          const optional<std::string>& type,
                          const optional<time_point>& startTime,
                          const optional<time_point>& endTime,
                          const optional<std::string>& shipper,
                          const optional<std::string>& driver,
                          const optional<std::string>& status,
                          const optional<std::string>& description,
                          const optional<std::string>& version);
    ACTION edititem     ( const std::string& user,
                          const std::string& company,
                          const std::string& site,
                          const std::string& itemId,
                          std::map<std::string, std::string> metadata,
                          const std::string& action,
                          const std::string& actionId,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,
                          const optional<int64_t>& quantity,
                          const optional<int64_t>& delta,
                          const optional<std::string>& product,
                          const optional<std::string>& version);
    ACTION editmachine  ( const std::string& user,
                          const std::string& company,
                          const std::string& machineId,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,
                          const optional<std::string>& site,
                          const optional<std::string>& name,
                          const optional<std::string>& description,
                          const optional<std::string>& version);
    ACTION editprocess  ( const std::string& user,
                          const std::string& company,
                          const std::string& processId,
                          const std::map<std::string, ProductQuantity>& inputsDelta,
                          const std::map<std::string, ProductQuantity>& outputsDelta,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,
                          const optional<time_point>& startTime,
                          const optional<time_point>& endTime,
                          const optional<std::string>& machine,
                          const optional<std::string>& status,
                          const optional<std::string>& description,
                          const optional<std::string>& version);
    ACTION editproduct  ( const std::string& user,
                          const std::string& company,
                          const std::string& productId,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,
                          const optional<std::string>& uom,
                          const optional<std::string>& name,
                          const optional<std::string>& image,
                          const optional<std::string>& description,
                          const optional<std::string>& version);
    ACTION editrecipe   ( const std::string& user,
                          const std::string& company,
                          const std::string& recipeId,
                          const std::vector<ProductQuantity>& inputs,
                          const std::vector<ProductQuantity>& outputs,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,
                          const optional<std::string>& name,
                          const optional<std::string>& description,
                          const optional<std::string>& version);
    ACTION editsite     ( const std::string& user,
                          const std::string& company,
                          const std::string& siteId,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,
                          const optional<std::string>& name,
                          const optional<std::string>& description,
                          const optional<std::string>& version);
    ACTION edituser     ( const std::string& user,
                          const std::string& company,
                          const std::string& userId,
                          const std::vector<std::string>& permissions,
                          const std::vector<std::string>& certifications,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,
                          const optional<public_key>& key,
                          const optional<std::string>& firstName,
                          const optional<std::string>& lastName,
                          const optional<std::string>& email,
                          const optional<std::string>& description,
                          const optional<std::string>& version);

    // Delete
    ACTION deldelivery  ( const std::string& user,
                          const std::string& company,
                          const std::string& deliveryId,
                          const time_point&  timestamp );
    ACTION delitem      ( const std::string& user,
                          const std::string& company,
                          const std::string& site,
                          const std::string& itemId,
                          const std::string& action,
                          const std::string& actionId,
                          const time_point&  timestamp);
    ACTION delmachine   ( const std::string& user,
                          const std::string& company,
                          const std::string& machineId,
                          const time_point&  timestamp );
    ACTION delprocess   ( const std::string& user,
                          const std::string& company,
                          const std::string& processId,
                          const time_point&  timestamp );
    ACTION delproduct   ( const std::string& user,
                          const std::string& company,
                          const std::string& productId,
                          const time_point&  timestamp);
    ACTION delrecipe    ( const std::string& user,
                          const std::string& company,
                          const std::string& recipeId,
                          const time_point&  timestamp );
    ACTION delsite      ( const std::string& user,
                          const std::string& company,
                          const std::string& siteId,
                          const time_point&  timestamp );
    ACTION deluser      ( const std::string& user,
                          const std::string& company,
                          const std::string& userId,
                          const time_point&  timestamp);

    // Logging
    ACTION loginventory ( const std::string& user,
                          const std::string& company,
                          const std::string& item,
                          const std::string& site,
                          const std::string& product,
                          std::map<std::string, std::string> metadata,
                          const std::string& action,
                          const std::string& parentAction,
                          const std::string& parentActionId,
                          const time_point&  timestamp,
                          const std::string& version,
                          const int64_t&     oldQuantity,
                          const int64_t&     newQuantity);

    ACTION cleartable (const std::string& tableName);

    template <typename T>
    void cleanTable(){
            T db(_self, _self.value);
            while(db.begin() != db.end()){
                    auto itr = --db.end();
                    db.erase(itr);
            }
    }

    ACTION push(
      std::string verifydata,
      const signature &sig,
      const public_key &pk,
      std::string username,
      std::string company,
      std::string entity,
      std::string action,
      std::map<std::string, all_type> args, std::map<std::string, all_type> metadata
    );

    // Action wrappers
    using loginventory_action = action_wrapper<"loginventory"_n, &tracelytics::loginventory>;
    using newitem_action      = action_wrapper<"newitem"_n,      &tracelytics::newitem>;
    using edititem_action     = action_wrapper<"edititem"_n,     &tracelytics::edititem>;
    using delitem_action      = action_wrapper<"delitem"_n,      &tracelytics::delitem>;
    using editprocess_action  = action_wrapper<"editprocess"_n,  &tracelytics::editprocess>;

  private:
    TABLE Delivery {
      uint64_t index;
      std::string company;
      std::string deliveryId;
      std::string type;
      std::string fromSite;
      std::string toSite;
      time_point startTime;
      time_point endTime;
      std::string shipper;
      std::string driver;
      std::string status;
      std::string description;
      std::string version = "0.0.1";
      std::string createdBy;
      std::string updatedBy;
      time_point createdAt;
      time_point updatedAt;
      std::map<std::string, ProductQuantity> cargo;
      std::map<std::string, all_type> data;

      uint64_t primary_key() const { return index; };
      checksum256 by_checksum() const {
        std::string final_checksum_string = company + ";" + deliveryId;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_company() const {
        std::string final_checksum_string = company;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_type() const {
        std::string final_checksum_string = company + ";" + type;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_creator() const {
        std::string final_checksum_string = company + ";" + createdBy;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_updater() const {
        std::string final_checksum_string = company + ";" + updatedBy;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_status() const {
        std::string final_checksum_string = company + ";" + status;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_fromSite() const {
        std::string final_checksum_string = company + ";" + fromSite;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_toSite() const {
        std::string final_checksum_string = company + ";" + toSite;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      uint128_t by_latest() const {
        checksum256 hashAsFixedBytes = sha256(company.c_str(), company.size());
        uint64_t truncatedHash = truncate_sha256_to_uint64(hashAsFixedBytes);
        uint64_t count = updatedAt.elapsed.count();
        return truncatedHash | count;
      }
    };

    TABLE InventoryLog {
      uint64_t index;
      std::string user;
      std::string company;
      std::string item;
      std::string site;
      std::string product;
      std::map<std::string, std::string> metadata;
      std::string action;
      std::string parentAction;
      std::string parentActionId;
      time_point timestamp;
      int64_t oldQuantity;
      int64_t newQuantity;
      int64_t delta;
      checksum256 txid;
      std::string version = "0.0.1";
      std::map<std::string, all_type> data;

      uint64_t primary_key() const { return index; };
      checksum256 by_item() const {
        std::string final_checksum_string = company + ";" + item;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_company() const {
        std::string final_checksum_string = company;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_site() const {
        std::string final_checksum_string = company + ";" + site;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_product() const {
        std::string final_checksum_string = company + ";" + product;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_user() const {
        std::string final_checksum_string = company + ";" + user;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_site_and_product() const {
        std::string final_checksum_string = company + ";" + site + ";" + product;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
    };
    TABLE Item {
      uint64_t    index;
      std::string company;
      std::string site;
      std::string itemId;
      std::string product;
      int64_t     quantity;
      std::string createdBy;
      std::string updatedBy;
      time_point createdAt;
      time_point updatedAt;
      std::map<std::string, std::string> metadata;
      std::string version = "0.0.1";
      std::map<std::string, all_type> data;

      uint64_t primary_key() const { return index; };
      checksum256 by_checksum() const {
        std::string final_checksum_string = company + ";" + itemId;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_company() const {
        std::string final_checksum_string = company;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_site() const {
        std::string final_checksum_string = company + ";" + site;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_site_and_item() const {
        std::string final_checksum_string = company + ";" + site + ";" + itemId;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_product() const {
        std::string final_checksum_string = company + ";" + product;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_site_and_product() const {
        std::string final_checksum_string = company + ";" + site + ";" + product;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
    };

    TABLE Machine {
      uint64_t index;
      std::string company;
      std::string machineId;
      std::string name;
      std::string site;
      std::string description;
      std::string createdBy;
      std::string updatedBy;
      time_point createdAt;
      time_point updatedAt;
      std::string version = "0.0.1";
      std::vector<std::string> certifications;
      std::map<std::string, all_type> data;

      uint64_t primary_key() const { return index; };
      checksum256 by_checksum() const {
        std::string final_checksum_string = company + ";" + machineId;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_company() const {
        std::string final_checksum_string = company;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_site() const {
        std::string final_checksum_string = company + ";" + site;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
    };

    TABLE Process {
      uint64_t index;
      std::string company;
      std::string processId;
      std::string type;
      std::string machine;
      std::string site;
      time_point startTime;
      time_point endTime;
      std::string status;
      std::string description;
      std::string createdBy;
      std::string updatedBy;
      time_point createdAt;
      time_point updatedAt;
      std::string version = "0.0.1";
      std::map<std::string, ProductQuantity> inputs;
      std::map<std::string, ProductQuantity> outputs;
      std::map<std::string, all_type> data;

      uint64_t primary_key() const { return index; };
      checksum256 by_checksum() const {
        std::string final_checksum_string = company + ";" + processId;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_company() const {
        std::string final_checksum_string = company;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_type() const {
        std::string final_checksum_string = company + ";" + type;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_creator() const {
        std::string final_checksum_string = company + ";" + createdBy;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_updater() const {
        std::string final_checksum_string = company + ";" + updatedBy;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_site() const {
        std::string final_checksum_string = company + ";" + site;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_machine() const {
        std::string final_checksum_string = company + ";" + machine;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
    };

    TABLE Product {
      uint64_t index;
      std::string company;
      std::string productId;
      std::string name;
      std::string uom;
      std::string image;
      std::string description;
      std::string createdBy;
      std::string updatedBy;
      time_point createdAt;
      time_point updatedAt;
      std::string version = "0.0.1";
      std::map<std::string, all_type> data;

      uint64_t primary_key() const { return index; };
      checksum256 by_checksum() const {
        std::string final_checksum_string = company + ";" + productId;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_company() const {
        std::string final_checksum_string = company;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
    };
    TABLE Recipe {
      uint64_t index;
      std::string company;
      std::string recipeId;
      std::string name;
      std::string description;
      std::string createdBy;
      std::string updatedBy;
      time_point createdAt;
      time_point updatedAt;
      std::string version = "0.0.1";
      std::vector<ProductQuantity> inputs;
      std::vector<ProductQuantity> outputs;
      std::map<std::string, all_type> data;

      uint64_t primary_key() const { return index; };
      checksum256 by_checksum() const {
        std::string final_checksum_string = company + ";" + recipeId;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_company() const {
        std::string final_checksum_string = company + ";" + company;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
    };
    TABLE Site {
      uint64_t index;
      std::string company;
      std::string siteId;
      std::string name;
      std::string description;
      std::string createdBy;
      std::string updatedBy;
      time_point createdAt;
      time_point updatedAt;
      std::string version = "0.0.1";
      std::map<std::string, all_type> data;

      uint64_t primary_key() const { return index; };
      checksum256 by_checksum() const {
        std::string final_checksum_string = company + ";" + siteId;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_company() const {
        std::string final_checksum_string = company;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
    };
    TABLE User {
      uint64_t index;
      std::string company;
      std::string userId;
      public_key key;
      uint64_t nonce = 0;
      std::string firstName;
      std::string lastName;
      std::string email;
      std::vector<std::string> permissions;
      std::vector<std::string> certifications;
      std::string description;
      std::string createdBy;
      std::string updatedBy;
      time_point createdAt;
      time_point updatedAt;
      std::string version = "0.0.1";
      std::map<std::string, all_type> data;

      uint64_t primary_key() const { return index; };
      checksum256 by_checksum() const {
        std::string final_checksum_string = userId;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_company() const {
        std::string final_checksum_string = company;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_fullname() const {
        std::string final_checksum_string = company + ";" + firstName + ";" + lastName;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }

      EOSLIB_SERIALIZE( User, (index)(company)(userId)(key)(nonce)(firstName)(lastName)(email)(permissions)(certifications)(description)(createdBy)(updatedBy)(createdAt)(updatedAt)(version)(data) )
    };

    typedef multi_index<"delivery"_n, Delivery,
      indexed_by<"bychecksum"_n, const_mem_fun<Delivery, checksum256, &Delivery::by_checksum>>,
      indexed_by<"bycompany"_n,  const_mem_fun<Delivery, checksum256, &Delivery::by_company>>,
      indexed_by<"bycreator"_n,  const_mem_fun<Delivery, checksum256, &Delivery::by_creator>>,
      indexed_by<"byupdater"_n,  const_mem_fun<Delivery, checksum256, &Delivery::by_updater>>,
      indexed_by<"bystatus"_n,   const_mem_fun<Delivery, checksum256, &Delivery::by_status>>,
      indexed_by<"byfromsite"_n, const_mem_fun<Delivery, checksum256, &Delivery::by_fromSite>>,
      indexed_by<"bytosite"_n,   const_mem_fun<Delivery, checksum256, &Delivery::by_toSite>>
    > delivery_table;
    typedef multi_index<"inventorylog"_n, InventoryLog,
      indexed_by<"byitem"_n,     const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_item>>,
      indexed_by<"bycompany"_n,  const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_company>>,
      indexed_by<"bysite"_n,     const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_site>>,
      indexed_by<"byproduct"_n,  const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_product>>,
      indexed_by<"byuser"_n,     const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_user>>,
      indexed_by<"bysiteprod"_n, const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_site_and_product>>
    > inventory_log_table;
    typedef multi_index<"item"_n, Item,
      indexed_by<"bychecksum"_n, const_mem_fun<Item, checksum256, &Item::by_checksum>>,
      indexed_by<"bycompany"_n,  const_mem_fun<Item, checksum256, &Item::by_company>>,
      indexed_by<"bysite"_n,     const_mem_fun<Item, checksum256, &Item::by_site>>,
      indexed_by<"bysiteitem"_n, const_mem_fun<Item, checksum256, &Item::by_site_and_item>>,
      indexed_by<"byproduct"_n,  const_mem_fun<Item, checksum256, &Item::by_product>>,
      indexed_by<"bysiteprod"_n, const_mem_fun<Item, checksum256, &Item::by_site_and_product>>
    > item_table;
    typedef multi_index<"machine"_n, Machine,
      indexed_by<"bychecksum"_n, const_mem_fun<Machine, checksum256, &Machine::by_checksum>>,
      indexed_by<"bycompany"_n,  const_mem_fun<Machine, checksum256, &Machine::by_company>>,
      indexed_by<"bysite"_n,     const_mem_fun<Machine, checksum256, &Machine::by_site>>
    > machine_table;
    typedef multi_index<"process"_n, Process,
      indexed_by<"bychecksum"_n, const_mem_fun<Process, checksum256, &Process::by_checksum>>,
      indexed_by<"bycompany"_n,  const_mem_fun<Process, checksum256, &Process::by_company>>,
      indexed_by<"bytype"_n,     const_mem_fun<Process, checksum256, &Process::by_type>>,
      indexed_by<"bycreator"_n,  const_mem_fun<Process, checksum256, &Process::by_creator>>,
      indexed_by<"byupdater"_n,  const_mem_fun<Process, checksum256, &Process::by_updater>>,
      indexed_by<"bymachine"_n,  const_mem_fun<Process, checksum256, &Process::by_machine>>,
      indexed_by<"bysite"_n,     const_mem_fun<Process, checksum256, &Process::by_site>>
    > process_table;
    typedef multi_index<"product"_n, Product,
      indexed_by<"bychecksum"_n, const_mem_fun<Product, checksum256, &Product::by_checksum>>,
      indexed_by<"bycompany"_n,  const_mem_fun<Product, checksum256, &Product::by_company>>
    > product_table;
    typedef multi_index<"recipe"_n, Recipe,
      indexed_by<"bychecksum"_n, const_mem_fun<Recipe, checksum256, &Recipe::by_checksum>>,
      indexed_by<"bycompany"_n,  const_mem_fun<Recipe, checksum256, &Recipe::by_company>>
    > recipe_table;
    typedef multi_index<"site"_n, Site,
      indexed_by<"bychecksum"_n, const_mem_fun<Site, checksum256, &Site::by_checksum>>,
      indexed_by<"bycompany"_n,  const_mem_fun<Site, checksum256, &Site::by_company>>
    > site_table;
    typedef multi_index<"user"_n, User,
      indexed_by<"bychecksum"_n, const_mem_fun<User, checksum256, &User::by_checksum>>,
      indexed_by<"bycompany"_n,  const_mem_fun<User, checksum256, &User::by_company>>
    > user_table;

    void ec_verify(std::string data, const signature &sig, const public_key &pk);
    void verify_auth(std::string company, std::string username, std::string entity, std::string action, std::string verifydata);
    std::vector<std::string> split(std::string str, std::string token);
    std::string to_hex(const char* d, uint32_t s);
    static uint64_t truncate_sha256_to_uint64(const checksum256& sha256);

    void upsertitem(
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
    );
};