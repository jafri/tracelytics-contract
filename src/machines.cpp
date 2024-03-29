#include "tracelytics/tracelytics.hpp"

/**
 * Create New Machine
 **/
void tracelytics::newmachine (
    const std::string& user,
    const std::string& company,
    const std::string& machineId,
    const std::string& site,
    const time_point& timestamp,
    const std::map<std::string, std::string>& data,

    const optional<std::string>& name,
    const optional<std::string>& description,
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),      "user is missing.");
    check(!company.empty(),   "company is missing.");
    check(!machineId.empty(), "machine ID is missing.");
    check(!site.empty(),      "site is missing.");

    // Site exists
    check_site_exists(company, site);

    // Access table and make sure machine doesnt exist
    auto machines_bycompandid = _machines.get_index<eosio::name("bycompandid")>();
    auto machine = machines_bycompandid.find(Checksum::MACHINE(company, machineId));
    check(machine == machines_bycompandid.end(), "machine already exists");

    // Create new machine
    _machines.emplace(get_self(), [&](auto& m) {
        m.index      = _machines.available_primary_key();
        m.createdBy  = user;
        m.updatedBy  = user;
        m.createdAt  = timestamp;
        m.updatedAt  = timestamp;

        m.company    = company;
        m.machineId  = machineId;
        m.site       = site;

        // Optional
        if (name)        m.name        = *name;
        if (description) m.description = *description;
        if (version)     m.version     = *version;
    });
}

/**
 * Edit Machine
 *
 * Params:
 *  - company (name)
 *  - machineId (string)
 *  - name (string) (optional)
 *  - site (string) (optional)
 **/
void tracelytics::editmachine (
    const std::string& user,
    const std::string& company,
    const std::string& machineId,
    const time_point& timestamp,
    const std::map<std::string, std::string>& data,

    const optional<std::string>& site,
    const optional<std::string>& name,
    const optional<std::string>& description,
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),      "user is missing.");
    check(!company.empty(),   "company is missing.");
    check(!machineId.empty(), "machine ID is missing.");

    // Access table and make sure machine exists
    auto machines_bycompandid = _machines.get_index<eosio::name("bycompandid")>();
    auto machine = machines_bycompandid.find(Checksum::MACHINE(company, machineId));
    check(machine != machines_bycompandid.end(), "machine does not exist.");
    check(machineId == machine->machineId, "machine mismatch");

    if (user != ADMIN) {
        check(company == machine->company, "only employees of " + machine->company + " can edit the machine.");
    }

    // Edit Machine
    machines_bycompandid.modify(machine, get_self(), [&](auto& m) {
        m.updatedBy = user;
        m.updatedAt = timestamp;

        // Optional
        if (name)        m.name        = *name;
        if (site)        m.site        = *site;
        if (description) m.description = *description;
        if (version)     m.version     = *version;
    });
}

/**
 * Delete Machine
 **/
void tracelytics::delmachine (
    const std::string& user,
    const std::string& company,
    const std::string& machineId,
    const time_point&  timestamp
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),      "user is missing.");
    check(!company.empty(), "company is missing.");
    check(!machineId.empty(), "machine ID is missing.");

    // Access table and make sure machine exists
    auto machines_bycompandid = _machines.get_index<eosio::name("bycompandid")>();
    auto machine = machines_bycompandid.find(Checksum::MACHINE(company, machineId));
    check(machine != machines_bycompandid.end(), "machine does not exist.");
    check(machineId == machine->machineId, "machine mismatch");

    if (user != ADMIN) {
        check(company == machine->company, "only employees of " + machine->company + " can delete the machine.");
    }

    // Delete machine
    machines_bycompandid.erase(machine);
}
