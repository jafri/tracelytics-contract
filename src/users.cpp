#include "tracelytics/tracelytics.hpp"

/**
 * Create New User
 *
 * Params:
 *  - company (name)
 *  - userId (string)
 *  - key (string) (optional)
 *  - permissions (string) (optional)
 **/
void tracelytics::newuser (std::map<std::string, all_type> args) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company      =  std::get_if<name>       (  &args["company"] );
    auto userId       =  std::get_if<std::string>(  &args["userId"] );
    auto key          =  std::get_if<public_key> (  &args["key"] );
    auto permissions  =  std::get_if<std::string>(  &args["permissions"] );

    // Validation
    check(company != nullptr, "company is missing.");
    check(userId  != nullptr, "user id is missing.");

    // Generate checksum from userId
    checksum256 user_checksum = sha256(userId->c_str(), userId->size());

    // Access table and make sure user doesnt exist
    user_table users(get_self(), company->value);
    auto users_bychecksum = users.get_index<"bychecksum"_n>();
    auto user = users_bychecksum.find(user_checksum);
    check(user == users_bychecksum.end(), "user already exists");

    // Create new user
    users.emplace(get_self(), [&](auto& u) {
        u.id = users.available_primary_key();
        u.user_checksum = user_checksum;
        u.user_id = *userId;

        // Optional
        if (key         != nullptr) u.key         = *key;
        if (permissions != nullptr) u.permissions = split(*permissions, ",");
    });
}

/**
 * Edit User
 *
 * Params:
 *  - company (name)
 *  - userId (string)
 *  - key (string) (optional)
 *  - permissions (string) (optional)
 **/
void tracelytics::edituser (std::map<std::string, all_type> args) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company     = std::get_if<name>       ( &args["company"] );
    auto userId      = std::get_if<std::string>( &args["userId"] );
    auto key         = std::get_if<public_key> ( &args["key"] );
    auto permissions = std::get_if<std::string>( &args["permissions"] );

    // Validation
    check(company != nullptr, "company is missing.");
    check(userId != nullptr, "user id is missing.");

    // Generate checksum from userId
    checksum256 user_checksum = sha256( userId->c_str(), userId->size() );

    // Access table and make sure user exists
    user_table users(get_self(), company->value);
    auto users_bychecksum = users.get_index<"bychecksum"_n>();
    auto user = users_bychecksum.find( user_checksum );
    check(user != users_bychecksum.end(), "user does not exist.");

    // Edit User
    users_bychecksum.modify(user, get_self(), [&](auto& u) {
        // Optional
        if (key         != nullptr) u.key         = *key;
        if (permissions != nullptr) u.permissions = split(*permissions, ",");
    });
}

/**
 * Delete User
 *
 * Params:
 *  - company (name)
 *  - userId (string)
 **/
void tracelytics::deluser (std::map<std::string, all_type> args) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company = std::get_if<name>       ( &args["company"] );
    auto userId  = std::get_if<std::string>( &args["userId"] );

    // Validation
    check(company != nullptr, "company is missing.");
    check(userId != nullptr, "user id is missing.");

    // Generate checksum from userId
    checksum256 user_checksum = sha256(userId->c_str(), userId->size());

    // Access table and make sure user exists
    user_table users(get_self(), company->value);
    auto users_bychecksum = users.get_index<"bychecksum"_n>();
    auto user = users_bychecksum.find(user_checksum);
    check(user != users_bychecksum.end(), "user does not exist.");

    // Delete user
    users_bychecksum.erase(user);
}
