#include "tracelytics/tracelytics.hpp"

/**
 * Create New Site
 **/
void tracelytics::newsite (
    const std::string& user,
    const std::string& company,
    const std::string& siteId,
    const time_point& timestamp,
    const std::map<std::string, all_type>& data,

    const optional<std::string>& name,
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
    std::string site_checksum_string = company + ";" + siteId;
    checksum256 site_checksum = sha256(site_checksum_string.c_str(), site_checksum_string.size());

    // Access table and make sure site doesnt exist
    site_table sites(get_self(), get_self().value);
    auto sites_bychecksum = sites.get_index<"bychecksum"_n>();
    auto site = sites_bychecksum.find(site_checksum);
    check(site == sites_bychecksum.end(), "site already exists");

    // Create new site
    sites.emplace(get_self(), [&](auto& s) {
        s.index     = sites.available_primary_key();
        s.createdBy = user;
        s.updatedBy = user;
        s.createdAt = timestamp;
        s.updatedAt = timestamp;

        s.company   = company;
        s.siteId    = siteId;

        // Optional
        if (name)        s.name        = *name;
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
    std::string site_checksum_string = company + ";" + siteId;
    checksum256 site_checksum = sha256(site_checksum_string.c_str(), site_checksum_string.size());

    // Access table and make sure site exists
    site_table sites(get_self(), get_self().value);
    auto sites_bychecksum = sites.get_index<"bychecksum"_n>();
    auto site = sites_bychecksum.find( site_checksum );
    check(site != sites_bychecksum.end(), "site does not exist.");

    // Edit Site
    sites_bychecksum.modify(site, get_self(), [&](auto& s) {
        s.updatedBy = user;
        s.updatedAt = timestamp;

        // Optional
        if (name)        s.name        = *name;
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
    std::string site_checksum_string = company + ";" + siteId;
    checksum256 site_checksum = sha256(site_checksum_string.c_str(), site_checksum_string.size());

    // Access table and make sure site exists
    site_table sites(get_self(), get_self().value);
    auto sites_bychecksum = sites.get_index<"bychecksum"_n>();
    auto site = sites_bychecksum.find(site_checksum);
    check(site != sites_bychecksum.end(), "site does not exist.");

    // Delete site
    sites_bychecksum.erase(site);
}
