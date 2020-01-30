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
    const std::map<std::string, std::string>& data,

    const optional<public_key>& key,
    const optional<std::string>& firstName,
    const optional<std::string>& lastName,
    const optional<std::string>& email,
    const optional<std::string>& phone,
    const optional<std::string>& description,
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),    "user is missing.");
    check(!company.empty(), "company is missing.");
    check(!userId.empty(),  "user ID is missing.");

    // Access table and make sure user doesnt exist
    auto users_byid = _users.get_index<eosio::name("byid")>();
    auto existing_user = users_byid.find(Checksum::USER(userId));
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
    const std::map<std::string, std::string>& data,

    const optional<public_key>& key,
    const optional<std::string>& firstName,
    const optional<std::string>& lastName,
    const optional<std::string>& email,
    const optional<std::string>& phone,
    const optional<std::string>& description,
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),    "user is missing.");
    check(!company.empty(), "company is missing.");
    check(!userId.empty(),  "user ID is missing.");

    // Access table and make sure user exists
    auto users_byid = _users.get_index<eosio::name("byid")>();
    auto existing_user = users_byid.find(Checksum::USER(userId));
    check(existing_user != users_byid.end(), "user does not exist.");
    check(userId == existing_user->userId, "user mismatch");

    if (user != ADMIN) {
        check(company == existing_user->company, "only employees of " + existing_user->company + " can edit the user.");
    }

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
    check(!userId.empty(),  "user ID is missing.");

    // Access table and make sure user exists
    auto users_byid = _users.get_index<eosio::name("byid")>();
    auto existing_user = users_byid.find(Checksum::USER(userId));
    check(existing_user != users_byid.end(), "user does not exist.");
    check(userId == existing_user->userId, "user mismatch");

    if (user != ADMIN) {
        check(company == existing_user->company, "only employees of " + existing_user->company + " can delete the user.");
    }

    // Delete user
    users_byid.erase(existing_user);
}
