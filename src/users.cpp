#include "tracelytics/tracelytics.hpp"

/**
 * Create New User
 **/
void tracelytics::newuser (
    const std::string& user,
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
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),    "user is missing.");
    check(!company.empty(), "company is missing.");
    check(!userId.empty(),  "user id is missing.");

    // Generate checksum from userId
    std::string user_checksum_string = userId;
    checksum256 user_checksum = sha256(user_checksum_string.c_str(), user_checksum_string.size());

    // Access table and make sure user doesnt exist
    auto users_byid = _users.get_index<eosio::name("byid")>();
    auto existing_user = users_byid.find(user_checksum);
    check(existing_user == users_byid.end(), "user already exists");

    // Create new user
    _users.emplace(get_self(), [&](auto& u) {
        u.index          = _users.available_primary_key();
        u.createdBy      = user;
        u.updatedBy      = user;
        u.createdAt      = timestamp;
        u.updatedAt      = timestamp;

        u.company        = company;
        u.userId         = userId;
        u.permissions    = permissions;
        u.certifications = certifications;

        // Optional
        if (firstName)   u.firstName   = *firstName;
        if (lastName)    u.lastName    = *lastName;
        if (email)       u.email       = *email;
        if (key)         u.key         = *key;
        if (description) u.description = *description;
        if (version)     u.version     = *version;
    });
}

/**
 * Edit User
 **/
void tracelytics::edituser (
    const std::string& user,
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
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),    "user is missing.");
    check(!company.empty(), "company is missing.");
    check(!userId.empty(),  "user id is missing.");

    // Generate checksum from userId
    std::string user_checksum_string = userId;
    checksum256 user_checksum = sha256(user_checksum_string.c_str(), user_checksum_string.size());

    // Access table and make sure user exists
    auto users_byid = _users.get_index<eosio::name("byid")>();
    auto existing_user = users_byid.find( user_checksum );
    check(existing_user != users_byid.end(), "user does not exist.");
    check(userId == existing_user->userId, "user mismatch");

    // Edit User
    users_byid.modify(existing_user, get_self(), [&](auto& u) {
        u.updatedBy      = user;
        u.updatedAt      = timestamp;

        u.permissions    = permissions;
        u.certifications = certifications;

        // Optional
        if (key)         u.key         = *key;
        if (firstName)   u.firstName   = *firstName;
        if (lastName)    u.lastName    = *lastName;
        if (email)       u.email       = *email;
        if (description) u.description = *description;
        if (version)     u.version     = *version;
    });
}

/**
 * Delete User
 **/
void tracelytics::deluser (
    const std::string& user,
    const std::string& company,
    const std::string& userId,
    const time_point&  timestamp
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),    "user is missing.");
    check(!company.empty(), "company is missing.");
    check(!userId.empty(),  "user id is missing.");

    // Generate checksum from userId
    std::string user_checksum_string = userId;
    checksum256 user_checksum = sha256(user_checksum_string.c_str(), user_checksum_string.size());

    // Access table and make sure user exists
    auto users_byid = _users.get_index<eosio::name("byid")>();
    auto existing_user = users_byid.find(user_checksum);
    check(existing_user != users_byid.end(), "user does not exist.");
    check(userId == existing_user->userId, "user mismatch");

    // Delete user
    users_byid.erase(existing_user);
}
