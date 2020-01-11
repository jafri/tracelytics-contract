#include "tracelytics/tracelytics.hpp"

/**
 * Create New company
 **/
void tracelytics::newcompany (
    const std::string& user,
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
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),      "user is missing.");
    check(!company.empty(),   "company is missing.");
    check(!companyId.empty(), "company ID is missing.");
    check(!name.empty(),      "name is missing.");

    // Access table and make sure company doesnt exist
    auto companies_byid = _companies.get_index<eosio::name("byid")>();
    auto company_itr = companies_byid.find(Checksum::COMPANY(companyId));
    check(company_itr == companies_byid.end(), "company already exists");

    // Create new company
    _companies.emplace(get_self(), [&](auto& d) {
        d.index      = _companies.available_primary_key();
        d.createdBy  = user;
        d.updatedBy  = user;
        d.createdAt  = timestamp;
        d.updatedAt  = timestamp;

        d.companyId = companyId;
        d.name      = name;

        // Optional
        if (legalName)     d.legalName     = *legalName;
        if (country)       d.country       = *country;
        if (contact)       d.contact       = *contact;
        if (phone)         d.phone         = *phone;
        if (email)         d.email         = *email;
        if (fax)           d.fax           = *fax;
        if (customerSince) d.customerSince = *customerSince;
        if (currentClient) d.currentClient = *currentClient;
        if (status)        d.status        = *status;
        if (description)   d.description   = *description;
        if (version)       d.version       = *version;
    });
}


/**
 * Edit company
 **/
void tracelytics::editcompany (
    const std::string& user,
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
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),       "user is missing.");
    check(!company.empty(),    "company is missing.");
    check(!companyId.empty(),  "company ID is missing.");

    // Access table and make sure company exists
    auto companies_byid = _companies.get_index<eosio::name("byid")>();
    auto company_itr = companies_byid.find(Checksum::COMPANY(companyId));
    check(company_itr != companies_byid.end(), "company does not exist.");
    check(companyId == company_itr->companyId, "company mismatch");

    // Edit company
    companies_byid.modify(company_itr, get_self(), [&](auto& d) {
        d.updatedBy = user;
        d.updatedAt = timestamp;

        // Optional
        if (name)          d.name          = *name;
        if (legalName)     d.legalName     = *legalName;
        if (country)       d.country       = *country;
        if (contact)       d.contact       = *contact;
        if (phone)         d.phone         = *phone;
        if (email)         d.email         = *email;
        if (fax)           d.fax           = *fax;
        if (customerSince) d.customerSince = *customerSince;
        if (currentClient) d.currentClient = *currentClient;
        if (status)        d.status        = *status;
        if (description)   d.description   = *description;
        if (version)       d.version       = *version;
    });
}

/**
 * Delete company
 **/
void tracelytics::delcompany (
    const std::string& user,
    const std::string& company,
    const std::string& companyId,
    const time_point&  timestamp
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),       "user is missing.");
    check(!company.empty(),    "company is missing.");
    check(!companyId.empty(),  "company ID is missing.");

    // Access table and make sure company exists
    auto companies_byid = _companies.get_index<eosio::name("byid")>();
    auto company_itr = companies_byid.find(Checksum::COMPANY(companyId));
    check(company_itr != companies_byid.end(), "company does not exist.");
    check(companyId == company_itr->companyId, "company mismatch");

    if (user != ADMIN) {
        check(false, "the user must be an admin to delete a company");
    }

    // Delete company
    companies_byid.erase(company_itr);
}
