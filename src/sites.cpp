#include "tracelytics/tracelytics.hpp"

/**
 * Create New Site
 **/
void tracelytics::newsite (
    const std::string& user,
    const std::string& company,
    const std::string& siteId,
    const std::string& siteCompany,
    const std::string& address,

    const time_point& timestamp,
    const std::map<std::string, all_type>& data,

    const optional<std::string>& name,
    const optional<std::string>& contact,
    const optional<std::string>& description,
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),        "user is missing.");
    check(!company.empty(),     "company is missing.");
    check(!siteId.empty(),      "site id is missing.");
    check(!siteCompany.empty(), "site company is missing.");
    check(!address.empty(),     "site address is missing.");

    // Generate checksum from siteId
    std::string site_checksum_string = siteId;
    checksum256 site_checksum = sha256(site_checksum_string.c_str(), site_checksum_string.size());

    // Access table and make sure site doesnt exist
    auto sites_byid = _sites.get_index<eosio::name("byid")>();
    auto site = sites_byid.find(site_checksum);
    check(site == sites_byid.end(), "site already exists");

    // Create new site
    _sites.emplace(get_self(), [&](auto& s) {
        s.index     = _sites.available_primary_key();
        s.createdBy = user;
        s.updatedBy = user;
        s.createdAt = timestamp;
        s.updatedAt = timestamp;

        s.company   = siteCompany;
        s.siteId    = siteId;
        s.address   = address;

        // Optional
        if (name)        s.name        = *name;
        if (contact)     s.contact     = *contact;
        if (description) s.description = *description;
        if (version)     s.version     = *version;
    });
}

/**
 * Edit Site
 **/
void tracelytics::editsite (
    const std::string& user,
    const std::string& company,
    const std::string& siteId,
    const time_point& timestamp,
    const std::map<std::string, all_type>& data,

    const optional<std::string>& name,
    const optional<std::string>& address,
    const optional<std::string>& contact,
    const optional<std::string>& description,
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),    "user is missing.");
    check(!company.empty(), "company is missing.");
    check(!siteId.empty(),  "site id is missing.");

    // Generate checksum from siteId
    std::string site_checksum_string = siteId;
    checksum256 site_checksum = sha256(site_checksum_string.c_str(), site_checksum_string.size());

    // Access table and make sure site exists
    auto sites_byid = _sites.get_index<eosio::name("byid")>();
    auto site = sites_byid.find( site_checksum );
    check(site != sites_byid.end(), "site does not exist.");
    check(siteId == site->siteId, "site mismatch");

    // Edit Site
    sites_byid.modify(site, get_self(), [&](auto& s) {
        s.updatedBy = user;
        s.updatedAt = timestamp;

        // Optional
        if (name)        s.name        = *name;
        if (address)     s.address     = *address;
        if (contact)     s.contact     = *contact;
        if (description) s.description = *description;
        if (version)     s.version     = *version;
    });
}

/**
 * Delete Site
 **/
void tracelytics::delsite (
    const std::string& user,
    const std::string& company,
    const std::string& siteId,
    const time_point&  timestamp
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),    "user is missing.");
    check(!company.empty(), "company is missing.");
    check(!siteId.empty(),  "site id is missing.");

    // Generate checksum from siteId
    std::string site_checksum_string = siteId;
    checksum256 site_checksum = sha256(site_checksum_string.c_str(), site_checksum_string.size());

    // Access table and make sure site exists
    auto sites_byid = _sites.get_index<eosio::name("byid")>();
    auto site = sites_byid.find(site_checksum);
    check(site != sites_byid.end(), "site does not exist.");
    check(siteId == site->siteId, "site mismatch");

    // Delete site
    sites_byid.erase(site);
}
