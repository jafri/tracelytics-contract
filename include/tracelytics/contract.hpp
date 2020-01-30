#pragma once

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
                          const std::map<std::string, std::string>& data,
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
                          const std::string& type,
                          std::map<std::string, ProductQuantity>& cargo,
                          const time_point& timestamp,
                          const std::map<std::string, std::string>& data,
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
                          const double& quantity,
                          std::map<std::string, std::string> metadata,
                          const std::string& action,
                          const std::string& actionId,
                          const time_point& timestamp,
                          const std::map<std::string, std::string>& data,
                          const optional<std::string>& version);
    ACTION newmachine   ( const std::string& user,
                          const std::string& company,
                          const std::string& machineId,
                          const std::string& site,
                          const time_point& timestamp,
                          const std::map<std::string, std::string>& data,
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
                          const std::map<std::string, std::string>& data,
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
                          const double& defaultPrice,
                          const std::string& defaultCurrency,
                          const time_point& timestamp,
                          const std::map<std::string, std::string>& data,
                          const optional<std::string>& image,
                          const optional<std::string>& description,
                          const optional<std::string>& version);
    ACTION newrecipe    ( const std::string& user,
                          const std::string& company,
                          const std::string& recipeId,
                          const std::vector<ProductQuantity>& inputs,
                          const std::vector<ProductQuantity>& outputs,
                          const time_point& timestamp,
                          const std::map<std::string, std::string>& data,
                          const optional<std::string>& name,
                          const optional<std::string>& description,
                          const optional<std::string>& version);
    ACTION newsite      ( const std::string& user,
                          const std::string& company,
                          const std::string& siteId,
                          const std::string& siteCompany,
                          const bool& tracked,
                          const time_point& timestamp,
                          const std::map<std::string, std::string>& data,
                          const optional<std::string>& name,
                          const optional<std::string>& address,
                          const optional<std::string>& contact,
                          const optional<std::string>& description,
                          const optional<std::string>& version);
    ACTION newuser      ( const std::string& user,
                          const std::string& company,
                          const std::string& userId,
                          const std::vector<std::string>& permissions,
                          const std::vector<std::string>& certifications,
                          const time_point& timestamp,
                          const std::map<std::string, std::string>& data,
                          const optional<public_key>& key,
                          const optional<std::string>& firstName,
                          const optional<std::string>& lastName,
                          const optional<std::string>& email,
                          const optional<std::string>& phone,
                          const optional<std::string>& description,
                          const optional<std::string>& version);

    // Patch
    ACTION editcompany  ( const std::string& user,
                          const std::string& company,
                          const std::string& companyId,
                          const time_point& timestamp,
                          const std::map<std::string, std::string>& data,
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
                          std::map<std::string, ProductQuantity>& cargoDeltas,
                          const time_point& timestamp,
                          const std::map<std::string, std::string>& data,

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
                          const std::map<std::string, std::string>& data,
                          const optional<double>& quantity,
                          const optional<double>& delta,
                          const optional<std::string>& product,
                          const optional<std::string>& delivery,
                          const optional<std::string>& version);
    ACTION editmachine  ( const std::string& user,
                          const std::string& company,
                          const std::string& machineId,
                          const time_point& timestamp,
                          const std::map<std::string, std::string>& data,
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
                          const std::map<std::string, std::string>& data,
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
                          const std::map<std::string, std::string>& data,

                          const optional<std::string>& name,
                          const optional<std::string>& uom,
                          const optional<double>& defaultPrice,
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
                          const std::map<std::string, std::string>& data,
                          const optional<std::string>& name,
                          const optional<std::string>& description,
                          const optional<std::string>& version);
    ACTION editsite     ( const std::string& user,
                          const std::string& company,
                          const std::string& siteId,
                          const bool& tracked,
                          const time_point& timestamp,
                          const std::map<std::string, std::string>& data,
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
                          const std::map<std::string, std::string>& data,
                          const optional<public_key>& key,
                          const optional<std::string>& firstName,
                          const optional<std::string>& lastName,
                          const optional<std::string>& email,
                          const optional<std::string>& phone,
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
                          const time_point&  timestamp,
                          const bool& cancel );
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
                          const time_point&  timestamp,
                          const bool& cancel );
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

    ACTION cleartable (const std::string& tableName);
    ACTION clearall ();

    template <typename T>
    void cleanTable(){
      T db(get_self(), get_self().value);
      auto itr = db.end();
      while(db.begin() != itr){
        itr = db.erase(--itr);
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
      std::map<std::string, std::string> data
    );

    // Logging
    ACTION loginventory ( const std::string& user,
                          const std::string& company,
                          const std::string& item,
                          const std::string& site,
                          const std::string& product,
                          const std::string& delivery,
                          std::map<std::string, std::string> metadata,
                          const std::string& action,
                          const std::string& parentAction,
                          const std::string& parentActionId,
                          const time_point&  timestamp,
                          const std::string& version,
                          const double&     oldQuantity,
                          const double&     newQuantity);

    // Action wrappers
    using loginventory_action = action_wrapper<name("loginventory"), &tracelytics::loginventory>;
    using newitem_action      = action_wrapper<name("newitem"),      &tracelytics::newitem>;
    using edititem_action     = action_wrapper<name("edititem"),     &tracelytics::edititem>;
    using delitem_action      = action_wrapper<name("delitem"),      &tracelytics::delitem>;
    using editprocess_action  = action_wrapper<name("editprocess"),  &tracelytics::editprocess>;
    using editdelivery_action = action_wrapper<name("editdelivery"), &tracelytics::editdelivery>;

    // General function
    static checksum256 SHA256(const std::string& stringToSha);

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
      std::string status = DeliveryStatus::LOADING;
      std::string description;
      std::string version = "0.0.1";
      std::string createdBy;
      std::string updatedBy;
      time_point createdAt;
      time_point updatedAt;
      std::map<std::string, std::string> data;

      uint64_t primary_key() const { return index;             };
      checksum256 by_id()    const { return SHA256(companyId); };
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
      std::string type;
      std::string description;
      std::string version = "0.0.1";
      std::string createdBy;
      std::string updatedBy;
      time_point createdAt;
      time_point updatedAt;
      std::map<std::string, ProductQuantity> cargo;
      std::map<std::string, std::string> data;

      uint64_t primary_key() const { return index; };
      std::string id() const { return deliveryId; };

      checksum256 by_id()                       const { return SHA256(deliveryId                                    ); };
      checksum256 by_route()                    const { return SHA256(deliveryId  + ";" + route                     ); };
      checksum256 by_from_company_site_status() const { return SHA256(fromCompany + ";" + fromSite + ";" + status   ); }; // Raptor Site A -> * && (Status)
      checksum256 by_to_company_site_status()   const { return SHA256(toCompany   + ";" + toSite   + ";" + status   ); }; // * -> Raptor Site A && (Status)
      checksum256 by_from_to_site()             const { return SHA256(fromCompany + ";" + fromSite + ";" + toSite   ); }; // Raptor -> Supply Site A
      checksum256 by_to_from_site()             const { return SHA256(toCompany   + ";" + toSite   + ";" + fromSite ); }; // Supply Site A -> Raptor
      checksum256 by_from_to_company()          const { return SHA256(fromCompany + ";" + toCompany                 ); }; // Raptor <-> Supplier || Raptor <-> Raptor

      checksum256 by_from_site()                const { return SHA256(fromCompany + ";" + fromSite                  ); }; // Raptor Site A -> *
      checksum256 by_to_site()                  const { return SHA256(toCompany   + ";" + toSite                    ); }; // * -> Raptor Site A
      checksum256 by_from_company()             const { return SHA256(fromCompany                                   ); }; // Raptor -> *
      checksum256 by_to_company()               const { return SHA256(toCompany                                     ); }; // * -> Raptor
      uint128_t   by_from_site_latest()         const { return ((uint128_t) truncate_sha256_to_uint64(SHA256(fromCompany + ";" + fromSite + ";" + status)) << 64) | updatedAt.elapsed.count(); };
      uint128_t   by_to_site_latest()           const { return ((uint128_t) truncate_sha256_to_uint64(SHA256(toCompany   + ";" + toSite + ";" + status))   << 64) | updatedAt.elapsed.count(); };
    };

    TABLE InventoryLog {
      uint64_t index;
      std::string user;
      std::string company;
      std::string item;
      std::string site;
      std::string product;
      std::string delivery;
      std::map<std::string, std::string> metadata;
      std::string action;
      std::string parentAction;
      std::string parentActionId;
      time_point timestamp;
      double oldQuantity;
      double newQuantity;
      double delta;
      checksum256 txid;
      std::string version = "0.0.1";
      std::map<std::string, std::string> data;

      uint64_t primary_key()                  const { return index;                                             };
      checksum256 by_item()                   const { return SHA256(company + ";" + item);                      };
      checksum256 by_company()                const { return SHA256(company);                                   };
      checksum256 by_site()                   const { return SHA256(company + ";" + site);                      };
      checksum256 by_product()                const { return SHA256(company + ";" + product);                   };
      checksum256 by_delivery()               const { return SHA256(delivery);                                  };
      checksum256 by_user()                   const { return SHA256(company + ";" + user);                      };
      checksum256 by_site_and_product()       const { return SHA256(company + ";" + site + ";" + product);      };
      checksum256 by_parent_action_id()       const { return SHA256(company + ";" + parentActionId);            };
      checksum256 by_user_and_parent_action() const { return SHA256(company + ";" + user + ";" + parentAction); };
    };
    TABLE Item {
      uint64_t    index;
      std::string company;
      std::string site;
      std::string itemId;
      std::string product;
      double     quantity;
      std::string delivery;
      std::string createdBy;
      std::string updatedBy;
      time_point createdAt;
      time_point updatedAt;
      std::map<std::string, std::string> metadata;
      std::string version = "0.0.1";
      std::map<std::string, std::string> data;

      uint64_t primary_key() const { return index; };

      checksum256 by_id()               const { return SHA256(itemId);                               }; // Specific item (UNIQUE)
      checksum256 by_company()          const { return SHA256(company);                              }; // All items across a company
      checksum256 by_site()             const { return SHA256(company + ";" + site);                 }; // All items at a specific site
      checksum256 by_product()          const { return SHA256(company + ";" + product);              }; // All items matching product at company
      checksum256 by_delivery()         const { return SHA256(delivery);                             }; // By the creator of the item
      checksum256 by_site_and_product() const { return SHA256(company + ";" + site + ";" + product); }; // All items matching product at site
      checksum256 by_creator()          const { return SHA256(company + ";" + createdBy);            }; // By the creator of the item
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
      std::map<std::string, std::string> data;

      uint64_t    primary_key()       const { return index;                             };
      checksum256 by_company_and_id() const { return SHA256(company + ";" + machineId); };
      checksum256 by_company()        const { return SHA256(company);                   };
      checksum256 by_site()           const { return SHA256(company + ";" + site);      };
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
      std::map<std::string, std::string> data;

      uint64_t    primary_key()       const { return index;                             };
      std::string id()                const { return processId;                         };

      checksum256 by_company_and_id() const { return SHA256(company + ";" + processId          );   };
      checksum256 by_company()        const { return SHA256(company                            );   };
      checksum256 by_type()           const { return SHA256(company + ";" + type               );   };
      checksum256 by_creator()        const { return SHA256(company + ";" + createdBy          );   };
      checksum256 by_updater()        const { return SHA256(company + ";" + updatedBy          );   };
      checksum256 by_site()           const { return SHA256(company + ";" + site               );   };
      checksum256 by_site_status()    const { return SHA256(company + ";" + site + ";" + status);   };
      checksum256 by_site_type()      const { return SHA256(company + ";" + site + ";" + type  );   };
      checksum256 by_machine()        const { return SHA256(company + ";" + machine            );   };
    };

    TABLE Product {
      uint64_t index;
      std::string productId;
      std::string name;
      std::string uom;
      std::string image;
      std::string description;
      double defaultPrice;
      std::string defaultCurrency;
      std::string createdBy;
      std::string updatedBy;
      time_point createdAt;
      time_point updatedAt;
      std::string version = "0.0.1";
      std::map<std::string, std::string> data;

      uint64_t    primary_key() const { return index;      };
      checksum256 by_id()       const { return SHA256(productId); };
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
      std::map<std::string, std::string> data;

      uint64_t    primary_key()       const { return index;                            };
      checksum256 by_company_and_id() const { return SHA256(company + ";" + recipeId); };
      checksum256 by_company()        const { return SHA256(company);                  };
    };
    TABLE Site {
      uint64_t index;
      std::string company;
      std::string siteId;
      std::string name;
      std::string address;
      std::string contact;
      std::string description;
      bool tracked;
      std::string createdBy;
      std::string updatedBy;
      time_point createdAt;
      time_point updatedAt;
      std::string version = "0.0.1";
      std::map<std::string, std::string> data;

      uint64_t    primary_key() const { return index;           };
      checksum256 by_id()       const { return SHA256(siteId);  };
      checksum256 by_company()  const { return SHA256(company); };
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
      std::string phone;
      std::vector<std::string> permissions;
      std::vector<std::string> certifications;
      std::string description;
      std::string createdBy;
      std::string updatedBy;
      time_point createdAt;
      time_point updatedAt;
      std::string version = "0.0.1";
      std::map<std::string, std::string> data;

      uint64_t   primary_key()             const { return index;                                              };
      checksum256 by_id()                  const { return SHA256(userId);                                     };
      checksum256 by_company()             const { return SHA256(company);                                    };
      checksum256 by_company_and_user_id() const { return SHA256(company + ";" + userId);                     };
      checksum256 by_fullname()            const { return SHA256(company + ";" + firstName + ";" + lastName); };
      checksum256 by_email()               const { return SHA256(company + ";" + email);                      };

      // ----------------------------------------
      // EDIT THIS WHEN YOU ADD A FIELD TO USER
      // ----------------------------------------
      EOSLIB_SERIALIZE( User, (index)(company)(userId)(key)(nonce)
                              (firstName)(lastName)(email)(phone)(permissions)
                              (certifications)(description)(createdBy)(updatedBy)(createdAt)
                              (updatedAt)(version)(data) )
    };

    typedef multi_index<eosio::name("company"), Company,
      indexed_by<name("byid"), const_mem_fun<Company, checksum256, &Company::by_id>>
    > company_table;
    typedef multi_index<eosio::name("delivery"), Delivery,
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
    typedef multi_index<eosio::name("inventorylog"), InventoryLog,
      indexed_by<name("byitem"),       const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_item>>,
      indexed_by<name("bycompany"),    const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_company>>,
      indexed_by<name("bysite"),       const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_site>>,
      indexed_by<name("byproduct"),    const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_product>>,
      indexed_by<name("bydelivery"),   const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_delivery>>,
      indexed_by<name("byuser"),       const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_user>>,
      indexed_by<name("bysiteprod"),   const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_site_and_product>>,
      indexed_by<name("byparentid"),   const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_parent_action_id>>,
      indexed_by<name("byuserparent"), const_mem_fun<InventoryLog, checksum256, &InventoryLog::by_user_and_parent_action>>
    > inventory_log_table;
    typedef multi_index<eosio::name("item"), Item,
      indexed_by<name("byid"),        const_mem_fun<Item, checksum256, &Item::by_id>>,
      indexed_by<name("bycompany"),   const_mem_fun<Item, checksum256, &Item::by_company>>,
      indexed_by<name("bysite"),      const_mem_fun<Item, checksum256, &Item::by_site>>,
      indexed_by<name("byproduct"),   const_mem_fun<Item, checksum256, &Item::by_product>>,
      indexed_by<name("bydelivery"),  const_mem_fun<Item, checksum256, &Item::by_delivery>>,
      indexed_by<name("bysiteprod"),  const_mem_fun<Item, checksum256, &Item::by_site_and_product>>,
      indexed_by<name("bycreator"),   const_mem_fun<Item, checksum256, &Item::by_creator>>
    > item_table;
    typedef multi_index<eosio::name("machine"), Machine,
      indexed_by<name("bycompandid"), const_mem_fun<Machine, checksum256, &Machine::by_company_and_id>>,
      indexed_by<name("bycompany"),   const_mem_fun<Machine, checksum256, &Machine::by_company>>,
      indexed_by<name("bysite"),      const_mem_fun<Machine, checksum256, &Machine::by_site>>
    > machine_table;
    typedef multi_index<eosio::name("process"), Process,
      indexed_by<name("bycompandid"),  const_mem_fun<Process, checksum256, &Process::by_company_and_id>>,
      indexed_by<name("bycompany"),    const_mem_fun<Process, checksum256, &Process::by_company>>,
      indexed_by<name("bytype"),       const_mem_fun<Process, checksum256, &Process::by_type>>,
      indexed_by<name("bycreator"),    const_mem_fun<Process, checksum256, &Process::by_creator>>,
      indexed_by<name("byupdater"),    const_mem_fun<Process, checksum256, &Process::by_updater>>,
      indexed_by<name("bysite"),       const_mem_fun<Process, checksum256, &Process::by_site>>,
      indexed_by<name("bysitestatus"), const_mem_fun<Process, checksum256, &Process::by_site_status>>,
      indexed_by<name("bysitetype"),   const_mem_fun<Process, checksum256, &Process::by_site_type>>,
      indexed_by<name("bymachine"),    const_mem_fun<Process, checksum256, &Process::by_machine>>
    > process_table;

    typedef multi_index<eosio::name("product"), Product,
      indexed_by<name("byid"),        const_mem_fun<Product, checksum256, &Product::by_id>>
    > product_table;
    typedef multi_index<eosio::name("recipe"), Recipe,
      indexed_by<name("bycompandid"), const_mem_fun<Recipe, checksum256, &Recipe:: by_company_and_id>>,
      indexed_by<name("bycompany"),   const_mem_fun<Recipe, checksum256, &Recipe:: by_company>>
    > recipe_table;
    typedef multi_index<eosio::name("site"), Site,
      indexed_by<name("byid"),        const_mem_fun<Site, checksum256, &Site::by_id>>,
      indexed_by<name("bycompany"),   const_mem_fun<Site, checksum256, &Site::by_company>>
    > site_table;
    typedef multi_index<eosio::name("user"), User,
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
    std::string checksum_to_hex(const checksum256& cs);
    static uint64_t truncate_sha256_to_uint64(const checksum256& sha256);

    inline void upsertitem(
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
    );

    Site check_site_exists (const std::string& company, const std::string& site);

    // Process
    inline void processcargo (
      Process& entity,
      std::map<std::string, ProductQuantity>& cargo,
      const std::map<std::string, ProductQuantity>& cargoDeltas,
      const std::string& user,
      const std::string& company,
      const std::string& action,
      const std::string& activity
    );
    inline InventoryDelta inventoryDeltaForCargo (
      const Process& entity,
      const ProductQuantity& productAndQuantity,
      const std::string& activity
    );
    // Delivery
    inline void processDelivery (
      Delivery& entity,
      std::map<std::string, ProductQuantity>& cargo,
      const std::string& activity,
      const std::string& deliveryAction
    );
};

namespace Checksum
{
  inline eosio::checksum256 COMPANY (std::string companyId)                      { return tracelytics::SHA256(companyId); }
  inline eosio::checksum256 MACHINE (std::string company, std::string machineId) { return tracelytics::SHA256(company + ";" + machineId); }
  inline eosio::checksum256 PROCESS (std::string company, std::string processId) { return tracelytics::SHA256(company + ";" + processId); }
  inline eosio::checksum256 PRODUCT (std::string productId)                      { return tracelytics::SHA256(productId); }
  inline eosio::checksum256 RECIPE  (std::string company, std::string recipeId)  { return tracelytics::SHA256(company + ";" + recipeId); }
  inline eosio::checksum256 SITE    (std::string siteId)                         { return tracelytics::SHA256(siteId); }
  inline eosio::checksum256 USER    (std::string userId)                         { return tracelytics::SHA256(userId); }
  inline eosio::checksum256 DELIVERY(std::string deliveryId, std::string route)  { return tracelytics::SHA256(deliveryId + ";" + route); }
  inline eosio::checksum256 ITEM    (std::string itemId)                         { return tracelytics::SHA256(itemId); }
}