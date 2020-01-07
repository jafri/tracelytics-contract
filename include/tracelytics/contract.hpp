#pragma once

#include <tracelytics/types.hpp>

CONTRACT tracelytics : public contract {
  using contract::contract;

  public:
    tracelytics( name receiver, name code, datastream<const char*> ds )
      : contract(receiver, code, ds),
        _companies(receiver, receiver.value),
        _deliveries(receiver, receiver.value),
        _inventory_logs(receiver, receiver.value),
        _items(receiver, receiver.value),
        _machines(receiver, receiver.value),
        _processes(receiver, receiver.value),
        _products(receiver, receiver.value),
        _recipes(receiver, receiver.value),
        _sites(receiver, receiver.value),
        _users(receiver, receiver.value) {}

    // Create
    ACTION newcompany   ( const std::string& user,
                          const std::string& company,
                          const std::string& companyId,
                          const std::string& name,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,
                          const optional<std::string>& legalName,
                          const optional<std::string>& country,
                          const optional<std::string>& contact,
                          const optional<std::string>& phone,
                          const optional<std::string>& email,
                          const optional<std::string>& fax,
                          const optional<time_point>& customerSince,
                          const optional<int64_t>& currentClient,
                          const optional<std::string>& status,
                          const optional<std::string>& description,
                          const optional<std::string>& version);
    ACTION newdelivery  ( const std::string& user,
                          const std::string& company,
                          const std::string& deliveryId,
                          const std::string& route,
                          const std::string& fromSite,
                          const std::string& toSite,
                          const std::string& fromCompany,
                          const std::string& toCompany,
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
                          const std::string& name,
                          const std::string& uom,
                          const int64_t& defaultPrice,
                          const std::string& defaultCurrency,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,
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
                          const std::string& siteCompany,
                          const std::string& address,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,
                          const optional<std::string>& name,
                          const optional<std::string>& contact,
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
    ACTION editcompany  ( const std::string& user,
                          const std::string& company,
                          const std::string& companyId,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,
                          const optional<std::string>& name,
                          const optional<std::string>& legalName,
                          const optional<std::string>& country,
                          const optional<std::string>& contact,
                          const optional<std::string>& phone,
                          const optional<std::string>& email,
                          const optional<std::string>& fax,
                          const optional<time_point>& customerSince,
                          const optional<int64_t>& currentClient,
                          const optional<std::string>& status,
                          const optional<std::string>& description,
                          const optional<std::string>& version);
    ACTION editdelivery ( const std::string& user,
                          const std::string& company,
                          const std::string& deliveryId,
                          const std::string& route,
                          const std::map<std::string, ProductQuantity>& cargoDeltas,
                          const time_point& timestamp,
                          const std::map<std::string, all_type>& data,

                          const optional<std::string>& toSite,
                          const optional<std::string>& toCompany,
                          const optional<time_point>&  startTime,
                          const optional<time_point>&  endTime,
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
                          const std::map<std::string, ProductQuantity>& inputDeltas,
                          const std::map<std::string, ProductQuantity>& outputDeltas,
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

                          const optional<std::string>& name,
                          const optional<std::string>& uom,
                          const optional<int64_t>& defaultPrice,
                          const optional<std::string>& defaultCurrency,
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
                          const optional<std::string>& address,
                          const optional<std::string>& contact,
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
    ACTION delcompany   ( const std::string& user,
                          const std::string& company,
                          const std::string& companyId,
                          const time_point&  timestamp);
    ACTION deldelivery  ( const std::string& user,
                          const std::string& company,
                          const std::string& deliveryId,
                          const std::string& route,
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
      T db(get_self(), get_self().value);
      auto itr = db.end();
      while(db.begin() != itr){
        itr = db.erase(itr);
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
    using loginventory_action = action_wrapper<name("loginventory"), &tracelytics::loginventory>;
    using newitem_action      = action_wrapper<name("newitem"),      &tracelytics::newitem>;
    using edititem_action     = action_wrapper<name("edititem"),     &tracelytics::edititem>;
    using delitem_action      = action_wrapper<name("delitem"),      &tracelytics::delitem>;
    using editprocess_action  = action_wrapper<name("editprocess"),  &tracelytics::editprocess>;
    using editdelivery_action = action_wrapper<name("editdelivery"), &tracelytics::editdelivery>;

  private:
    TABLE Company {
      uint64_t index;
      std::string companyId;
      std::string name;
      std::string legalName;
      std::string country;
      std::string contact;
      std::string phone;
      std::string email;
      std::string fax;
      time_point customerSince;
      int64_t currentClient = 0;
      std::string status;
      std::string description;
      std::string version = "0.0.1";
      std::string createdBy;
      std::string updatedBy;
      time_point createdAt;
      time_point updatedAt;
      std::map<std::string, all_type> data;

      uint64_t primary_key() const { return index; };
      checksum256 by_id() const {
        std::string final_checksum_string = companyId;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
    };

    TABLE Delivery {
      uint64_t index;
      std::string deliveryId;
      std::string route;
      std::string fromSite;
      std::string toSite;
      std::string fromCompany;
      std::string toCompany;
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
      std::string id() const { return deliveryId; };

      checksum256 by_id() const {
        std::string final_checksum_string = deliveryId;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_route() const {
        std::string final_checksum_string = deliveryId + ";" + route;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }

      // Raptor Site A -> * && (Status)
      checksum256 by_from_company_site_status() const {
        std::string final_checksum_string = fromCompany + ";" + fromSite + ":" + status;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      // * -> Raptor Site A && (Status)
      checksum256 by_to_company_site_status() const {
        std::string final_checksum_string = toCompany + ";" + toSite + ";" + status;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }

      // Raptor -> Supply Site A
      checksum256 by_from_to_site() const {
        std::string final_checksum_string = fromCompany + ";" + fromSite + ";" + toSite;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      // Supply Site A -> Raptor
      checksum256 by_to_from_site() const {
        std::string final_checksum_string = toCompany + ";" + toSite + ";" + fromSite;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      // Raptor <-> Supplier || Raptor <-> Raptor
      checksum256 by_from_to_company() const {
        std::string final_checksum_string = fromCompany + ";" + toCompany;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }

      // Raptor Site A -> *
      checksum256 by_from_site() const {
        std::string final_checksum_string = fromCompany + ";" + fromSite;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      // * -> Raptor Site A
      checksum256 by_to_site() const {
        std::string final_checksum_string = toCompany + ";" + toSite;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      // Raptor -> *
      checksum256 by_from_company() const {
        std::string final_checksum_string = fromCompany;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      // * -> Raptor
      checksum256 by_to_company() const {
        std::string final_checksum_string = toCompany;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }

      uint128_t by_from_site_latest() const {
        uint64_t truncatedHash = truncate_sha256_to_uint64(
          SHA256(fromCompany + ";" + fromSite)
        );
        uint64_t count = updatedAt.elapsed.count();
        return truncatedHash | count;
      }

      uint128_t by_to_site_latest() const {
        uint64_t truncatedHash = truncate_sha256_to_uint64(
          SHA256(toCompany + ";" + toSite)
        );
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
      checksum256 by_parent_action_id() const {
        std::string final_checksum_string = company + ";" + parentActionId;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_user_and_parent_action() const {
        std::string final_checksum_string = company + ";" + user + ";" + parentAction;
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

      // Specific item at a single site (UNIQUE)
      checksum256 by_site_and_item() const {
        std::string final_checksum_string = company + ";" + site + ";" + itemId;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      // Specific item across all sites of a company
      checksum256 by_company_and_id() const {
        std::string final_checksum_string = company + ";" + itemId;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      // All items across a company
      checksum256 by_company() const {
        std::string final_checksum_string = company;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      // All items at a specific site
      checksum256 by_site() const {
        std::string final_checksum_string = company + ";" + site;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      // All items matching product at company
      checksum256 by_product() const {
        std::string final_checksum_string = company + ";" + product;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      // All items matching product at site
      checksum256 by_site_and_product() const {
        std::string final_checksum_string = company + ";" + site + ";" + product;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      // By the creator of the item
      checksum256 by_creator() const {
        std::string final_checksum_string = company + ";" + createdBy;
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
      checksum256 by_company_and_id() const {
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
      std::string id() const { return processId; };
      checksum256 by_company_and_id() const {
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
      std::string productId;
      std::string name;
      std::string uom;
      std::string image;
      std::string description;
      int64_t defaultPrice;
      std::string defaultCurrency;
      std::string createdBy;
      std::string updatedBy;
      time_point createdAt;
      time_point updatedAt;
      std::string version = "0.0.1";
      std::map<std::string, all_type> data;

      uint64_t primary_key() const { return index; };
      checksum256 by_id() const {
        std::string final_checksum_string = productId;
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
      checksum256 by_company_and_id() const {
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
      std::string address;
      std::string contact;
      std::string description;
      std::string createdBy;
      std::string updatedBy;
      time_point createdAt;
      time_point updatedAt;
      std::string version = "0.0.1";
      std::map<std::string, all_type> data;

      uint64_t primary_key() const { return index; };
      checksum256 by_id() const {
        std::string final_checksum_string = siteId;
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
      checksum256 by_id() const {
        std::string final_checksum_string = userId;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_company() const {
        std::string final_checksum_string = company;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_company_and_user_id() const {
        std::string final_checksum_string = company + ";" + userId;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_fullname() const {
        std::string final_checksum_string = company + ";" + firstName + ";" + lastName;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }
      checksum256 by_email() const {
        std::string final_checksum_string = company + ";" + email;
        return sha256(final_checksum_string.c_str(), final_checksum_string.size());
      }

      EOSLIB_SERIALIZE( User, (index)(company)(userId)(key)(nonce)(firstName)(lastName)(email)(permissions)(certifications)(description)(createdBy)(updatedBy)(createdAt)(updatedAt)(version)(data) )
    };


    typedef multi_index<name("company"), Company,
      indexed_by<name("byid"), const_mem_fun<Company, checksum256, &Company::by_id>>
    > company_table;
    typedef multi_index<name("delivery"), Delivery,
      indexed_by<name("byid"),         const_mem_fun<Delivery, checksum256, &Delivery::by_id>>,
      indexed_by<name("byroute"),      const_mem_fun<Delivery, checksum256, &Delivery::by_route>>,
      indexed_by<name("fromcompstat"), const_mem_fun<Delivery, checksum256, &Delivery::by_from_company_site_status>>,
      indexed_by<name("tocompstat"),   const_mem_fun<Delivery, checksum256, &Delivery::by_to_company_site_status>>,
      indexed_by<name("fromtosite"),   const_mem_fun<Delivery, checksum256, &Delivery::by_from_to_site>>,
      indexed_by<name("tofromsite"),   const_mem_fun<Delivery, checksum256, &Delivery::by_to_from_site>>,
      indexed_by<name("fromtocomp"),   const_mem_fun<Delivery, checksum256, &Delivery::by_from_to_company>>,
      indexed_by<name("byfromsite"),   const_mem_fun<Delivery, checksum256, &Delivery::by_from_site>>,
      indexed_by<name("bytosite"),     const_mem_fun<Delivery, checksum256, &Delivery::by_to_site>>,
      indexed_by<name("byfromcomp"),   const_mem_fun<Delivery, checksum256, &Delivery::by_from_company>>,
      indexed_by<name("bytocomp"),     const_mem_fun<Delivery, checksum256, &Delivery::by_to_company>>,
      indexed_by<name("newfromsite"),  const_mem_fun<Delivery, uint128_t,   &Delivery::by_from_site_latest>>,
      indexed_by<name("newtosite"),    const_mem_fun<Delivery, uint128_t,   &Delivery::by_to_site_latest>>
    > delivery_table;
    typedef multi_index<name("inventorylog"), InventoryLog,
      indexed_by<name("byitem"),       const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_item>>,
      indexed_by<name("bycompany"),    const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_company>>,
      indexed_by<name("bysite"),       const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_site>>,
      indexed_by<name("byproduct"),    const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_product>>,
      indexed_by<name("byuser"),       const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_user>>,
      indexed_by<name("bysiteprod"),   const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_site_and_product>>,
      indexed_by<name("byparentid"),   const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_parent_action_id>>,
      indexed_by<name("byuserparent"), const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_user_and_parent_action>>
    > inventory_log_table;
    typedef multi_index<name("item"), Item,
      indexed_by<name("bysiteitem"),  const_mem_fun<Item, checksum256, &Item::by_site_and_item>>,
      indexed_by<name("bycompandid"), const_mem_fun<Item, checksum256, &Item::by_company_and_id>>,
      indexed_by<name("bycompany"),   const_mem_fun<Item, checksum256, &Item::by_company>>,
      indexed_by<name("bysite"),      const_mem_fun<Item, checksum256, &Item::by_site>>,
      indexed_by<name("byproduct"),   const_mem_fun<Item, checksum256, &Item::by_product>>,
      indexed_by<name("bysiteprod"),  const_mem_fun<Item, checksum256, &Item::by_site_and_product>>,
      indexed_by<name("bycreator"),   const_mem_fun<Item, checksum256, &Item::by_creator>>
    > item_table;
    typedef multi_index<name("machine"), Machine,
      indexed_by<name("bycompandid"), const_mem_fun<Machine, checksum256, &Machine::by_company_and_id>>,
      indexed_by<name("bycompany"),   const_mem_fun<Machine, checksum256, &Machine::by_company>>,
      indexed_by<name("bysite"),      const_mem_fun<Machine, checksum256, &Machine::by_site>>
    > machine_table;
    typedef multi_index<name("process"), Process,
      indexed_by<name("bycompandid"), const_mem_fun<Process, checksum256, &Process::by_company_and_id>>,
      indexed_by<name("bycompany"),   const_mem_fun<Process, checksum256, &Process::by_company>>,
      indexed_by<name("bytype"),      const_mem_fun<Process, checksum256, &Process::by_type>>,
      indexed_by<name("bycreator"),   const_mem_fun<Process, checksum256, &Process::by_creator>>,
      indexed_by<name("byupdater"),   const_mem_fun<Process, checksum256, &Process::by_updater>>,
      indexed_by<name("bymachine"),   const_mem_fun<Process, checksum256, &Process::by_machine>>,
      indexed_by<name("bysite"),      const_mem_fun<Process, checksum256, &Process::by_site>>
    > process_table;
    typedef multi_index<name("product"), Product,
      indexed_by<name("byid"),        const_mem_fun<Product, checksum256, &Product::by_id>>
    > product_table;
    typedef multi_index<name("recipe"), Recipe,
      indexed_by<name("bycompandid"), const_mem_fun<Recipe, checksum256, &Recipe:: by_company_and_id>>,
      indexed_by<name("bycompany"),   const_mem_fun<Recipe, checksum256, &Recipe:: by_company>>
    > recipe_table;
    typedef multi_index<name("site"), Site,
      indexed_by<name("byid"),        const_mem_fun<Site, checksum256, &Site::by_id>>,
      indexed_by<name("bycompany"),   const_mem_fun<Site, checksum256, &Site::by_company>>
    > site_table;
    typedef multi_index<name("user"), User,
      indexed_by<name("byid"),        const_mem_fun<User, checksum256, &User::by_id>>,
      indexed_by<name("bycompandid"), const_mem_fun<User, checksum256, &User::by_company_and_user_id>>,
      indexed_by<name("bycompany"),   const_mem_fun<User, checksum256, &User::by_company>>,
      indexed_by<name("byfullname"),  const_mem_fun<User, checksum256, &User::by_fullname>>,
      indexed_by<name("byemail"),     const_mem_fun<User, checksum256, &User::by_email>>
    > user_table;

    company_table       _companies;
    delivery_table      _deliveries;
    inventory_log_table _inventory_logs;
    item_table          _items;
    machine_table       _machines;
    process_table       _processes;
    product_table       _products;
    recipe_table        _recipes;
    site_table          _sites;
    user_table          _users;

    void ec_verify(std::string data, const signature &sig, const public_key &pk);
    void verify_auth(std::string company, std::string username, std::string entity, std::string action, std::string verifydata);
    std::vector<std::string> split(std::string str, std::string token);
    std::string to_hex(const char* d, uint32_t s);
    static uint64_t truncate_sha256_to_uint64(const checksum256& sha256);
    static checksum256 SHA256(const std::string& stringToSha);

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

    template <typename T>
    void processcargo (
      T& entity,
      std::map<std::string, ProductQuantity>& cargo,
      const std::map<std::string, ProductQuantity>& cargoDeltas,
      const std::string& user,
      const std::string& company,
      const std::string& action,
      const bool& skipUpsert
    );
};