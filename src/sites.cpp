#include "tracelytics/tracelytics.hpp"

/**
 * Create New Site
 *
 * Params:
 *  - company (name)
 *  - siteId (string)
 *  - name (string) (optional)
 **/
void tracelytics::newsite (std::map<std::string, all_type> args) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company = std::get_if<name>       (  &args["company"] );
    auto siteId  = std::get_if<std::string>(  &args["siteId"] );
    auto name    = std::get_if<std::string>(  &args["name"] );

    // Validation
    check(company != nullptr, "company is missing.");
    check(siteId  != nullptr, "site id is missing.");

    // Generate checksum from siteId
    checksum256 site_checksum = sha256(siteId->c_str(), siteId->size());

    // Access table and make sure site doesnt exist
    site_table sites(get_self(), company->value);
    auto sites_bychecksum = sites.get_index<"bychecksum"_n>();
    auto site = sites_bychecksum.find(site_checksum);
    check(site == sites_bychecksum.end(), "site already exists");

    // Create new site
    sites.emplace(get_self(), [&](auto& s) {
        s.id = sites.available_primary_key();
        s.site_checksum = site_checksum;
        s.site_id = *siteId;

        // Optional
        if (name != nullptr) s.name = *name;
    });
}

/**
 * Edit Site
 *
 * Params:
 *  - company (name)
 *  - siteId (string)
 *  - name (string) (optional)
 **/
void tracelytics::editsite (std::map<std::string, all_type> args) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company = std::get_if<name>       (  &args["company"] );
    auto siteId  = std::get_if<std::string>(  &args["siteId"] );
    auto name    = std::get_if<std::string> (  &args["name"] );

    // Validation
    check(company != nullptr, "company is missing.");
    check(siteId != nullptr, "site id is missing.");

    // Generate checksum from siteId
    checksum256 site_checksum = sha256( siteId->c_str(), siteId->size() );

    // Access table and make sure site exists
    site_table sites(get_self(), company->value);
    auto sites_bychecksum = sites.get_index<"bychecksum"_n>();
    auto site = sites_bychecksum.find( site_checksum );
    check(site != sites_bychecksum.end(), "site does not exist.");

    // Edit Site
    sites_bychecksum.modify(site, get_self(), [&](auto& s) {
        // Optional
        if (name != nullptr) s.name = *name;
    });
}

/**
 * Delete Site
 *
 * Params:
 *  - company (name)
 *  - siteId (string)
 **/
void tracelytics::delsite (std::map<std::string, all_type> args) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company =  std::get_if<name>       (  &args["company"] );
    auto siteId  =  std::get_if<std::string>(  &args["siteId"] );

    // Validation
    check(company != nullptr, "company is missing.");
    check(siteId != nullptr, "site id is missing.");

    // Generate checksum from siteId
    checksum256 site_checksum = sha256(siteId->c_str(), siteId->size());

    // Access table and make sure site exists
    site_table sites(get_self(), company->value);
    auto sites_bychecksum = sites.get_index<"bychecksum"_n>();
    auto site = sites_bychecksum.find(site_checksum);
    check(site != sites_bychecksum.end(), "site does not exist.");

    // Delete site
    sites_bychecksum.erase(site);
}
