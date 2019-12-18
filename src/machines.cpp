#include "tracelytics/tracelytics.hpp"

/**
 * Create New Machine
 *
 * Params:
 *  - company (name)
 *  - machineId (string)
 *  - name (string) (optional)
 *  - site (checksum256) (optional)
 **/
void tracelytics::newmachine (std::map<std::string, all_type> args) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company      =  std::get_if<name>       (  &args["company"] );
    auto machineId    =  std::get_if<std::string>(  &args["machineId"] );
    auto name         =  std::get_if<std::string>(  &args["name"] );
    auto site         =  std::get_if<checksum256>(  &args["site"] );

    // Validation
    check(company != nullptr, "company is missing.");
    check(machineId  != nullptr, "machine id is missing.");

    // Generate checksum from machineId
    checksum256 machine_checksum = sha256(machineId->c_str(), machineId->size());

    // Access table and make sure machine doesnt exist
    machine_table machines(get_self(), company->value);
    auto machines_bychecksum = machines.get_index<"bychecksum"_n>();
    auto machine = machines_bychecksum.find(machine_checksum);
    check(machine == machines_bychecksum.end(), "machine already exists");

    // Create new machine
    machines.emplace(get_self(), [&](auto& m) {
        m.id = machines.available_primary_key();
        m.machine_checksum = machine_checksum;
        m.machine_id = *machineId;

        // Optional
        if (name != nullptr) m.name = *name;
        if (site != nullptr) m.site = *site;
    });
}

/**
 * Edit Machine
 *
 * Params:
 *  - company (name)
 *  - machineId (string)
 *  - name (string) (optional)
 *  - site (checksum256) (optional)
 **/
void tracelytics::editmachine (std::map<std::string, all_type> args) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company      =  std::get_if<name>       (  &args["company"] );
    auto machineId    =  std::get_if<std::string>(  &args["machineId"] );
    auto name         =  std::get_if<std::string> (  &args["name"] );
    auto site         =  std::get_if<checksum256>(  &args["site"] );

    // Validation
    check(company != nullptr, "company is missing.");
    check(machineId != nullptr, "machine id is missing.");

    // Generate checksum from machineId
    checksum256 machine_checksum = sha256( machineId->c_str(), machineId->size() );

    // Access table and make sure machine exists
    machine_table machines(get_self(), company->value);
    auto machines_bychecksum = machines.get_index<"bychecksum"_n>();
    auto machine = machines_bychecksum.find( machine_checksum );
    check(machine != machines_bychecksum.end(), "machine does not exist.");

    // Edit Machine
    machines_bychecksum.modify(machine, get_self(), [&](auto& m) {
        // Optional
        if (name != nullptr) m.name = *name;
        if (site != nullptr) m.site = *site;
    });
}

/**
 * Delete Machine
 *
 * Params:
 *  - company (name)
 *  - machineId (string)
 **/
void tracelytics::delmachine (std::map<std::string, all_type> args) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company      =  std::get_if<name>       (  &args["company"] );
    auto machineId    =  std::get_if<std::string>(  &args["machineId"] );

    // Validation
    check(company != nullptr, "company is missing.");
    check(machineId != nullptr, "machine id is missing.");

    // Generate checksum from machineId
    checksum256 machine_checksum = sha256(machineId->c_str(), machineId->size());

    // Access table and make sure machine exists
    machine_table machines(get_self(), company->value);
    auto machines_bychecksum = machines.get_index<"bychecksum"_n>();
    auto machine = machines_bychecksum.find(machine_checksum);
    check(machine != machines_bychecksum.end(), "machine does not exist.");

    // Delete machine
    machines_bychecksum.erase(machine);
}
