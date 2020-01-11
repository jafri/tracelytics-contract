#include "tracelytics/tracelytics.hpp"

/**
 * Create New Site
 **/
void tracelytics::newsite (
    const std::string& user,
    const std::string& company,
    const std::string& siteId,
    const std::string& siteCompany,
    const bool& tracked,

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
    check(!user.empty(),        "user is missing.");
    check(!company.empty(),     "company is missing.");
    check(!siteId.empty(),      "site ID is missing.");
    check(!siteCompany.empty(), "site company is missing.");

    // Access table and make sure site doesnt exist
    auto sites_byid = _sites.get_index<eosio::name("byid")>();
    auto site = sites_byid.find(Checksum::SITE(siteId));
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
        s.tracked   = tracked;

        // Optional
        if (name)        s.name        = *name;
        if (address)     s.address        = *address;
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
    const bool& tracked,
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
    check(!siteId.empty(),  "site ID is missing.");

    // Access table and make sure site exists
    auto sites_byid = _sites.get_index<eosio::name("byid")>();
    auto site = sites_byid.find(Checksum::SITE(siteId));
    check(site != sites_byid.end(), "site does not exist.");
    check(siteId == site->siteId, "site mismatch");
    check(user == ADMIN || company == site->company, "only employees of " + site->company + " can edit the site.");

    // Edit Site
    sites_byid.modify(site, get_self(), [&](auto& s) {
        s.updatedBy = user;
        s.updatedAt = timestamp;
        s.tracked = tracked;

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
    check(!siteId.empty(),  "site ID is missing.");

    // Access table and make sure site exists
    auto sites_byid = _sites.get_index<eosio::name("byid")>();
    auto site = sites_byid.find(Checksum::SITE(siteId));
    check(site != sites_byid.end(), "site does not exist.");
    check(siteId == site->siteId, "site mismatch");

    if (user != ADMIN) {
        check(company == site->company || user == site->createdBy, "only employees of " + site->company + " can delete the site.");
    }

    // Delete site
    sites_byid.erase(site);
}

tracelytics::Site tracelytics::check_site_exists (const std::string& company, const std::string& site) {
    auto sites_byid = _sites.get_index<eosio::name("byid")>();
    auto site_itr = sites_byid.find(Checksum::SITE(site));
    check(site_itr != sites_byid.end(), "Site " + site + " does not exist.");
    check(site_itr->company == company, "Company " + company + " does not have site " + site);
    return *site_itr;
}
