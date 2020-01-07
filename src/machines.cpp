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
    const std::map<std::string, all_type>& data,

    const optional<std::string>& name,
    const optional<std::string>& description,
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),      "user is missing.");
    check(!company.empty(),   "company is missing.");
    check(!machineId.empty(), "machine id is missing.");
    check(!site.empty(),      "site is missing.");

    // Generate checksum from machineId
    std::string machine_checksum_string = company + ";" + machineId;
    checksum256 machine_checksum = sha256(machine_checksum_string.c_str(), machine_checksum_string.size());

    // Access table and make sure machine doesnt exist
    auto machines_bycompandid = _machines.get_index<eosio::name("bycompandid")>();
    auto machine = machines_bycompandid.find(machine_checksum);
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
    const std::map<std::string, all_type>& data,

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
    check(!machineId.empty(), "machine id is missing.");

    // Generate checksum from machineId
    std::string machine_checksum_string = company + ";" + machineId;
    checksum256 machine_checksum = sha256(machine_checksum_string.c_str(), machine_checksum_string.size());

    // Access table and make sure machine exists
    auto machines_bycompandid = _machines.get_index<eosio::name("bycompandid")>();
    auto machine = machines_bycompandid.find( machine_checksum );
    check(machine != machines_bycompandid.end(), "machine does not exist.");
    check(company == machine->company && machineId == machine->machineId, "machine mismatch");

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
    check(!machineId.empty(), "machine id is missing.");

    // Generate checksum from machineId
    std::string machine_checksum_string = company + ";" + machineId;
    checksum256 machine_checksum = sha256(machine_checksum_string.c_str(), machine_checksum_string.size());

    // Access table and make sure machine exists
    auto machines_bycompandid = _machines.get_index<eosio::name("bycompandid")>();
    auto machine = machines_bycompandid.find(machine_checksum);
    check(machine != machines_bycompandid.end(), "machine does not exist.");
    check(company == machine->company && machineId == machine->machineId, "machine mismatch");

    // Delete machine
    machines_bycompandid.erase(machine);
}
