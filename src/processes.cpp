#include "tracelytics/tracelytics.hpp"

/**
 * Create New Process
 **/
void tracelytics::newprocess (
    const std::string& user,
    const std::string& company,
    const std::string& processId,
    const std::string& type,
    const std::string& site,
    const time_point& startTime,
    const std::map<std::string, ProductQuantity>& inputs,
    const std::map<std::string, ProductQuantity>& outputs,
    const time_point& timestamp,
    const std::map<std::string, all_type>& data,

    const optional<time_point>& endTime,
    const optional<std::string>& machine,
    const optional<std::string>& status,
    const optional<std::string>& description,
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),      "user is missing.");
    check(!company.empty(),   "company is missing.");
    check(!processId.empty(), "process id is missing.");
    check(!type.empty(),      "type is missing.");
    check(!site.empty(),      "site is missing.");
    check(inputs.size() > 0,  "inputs are empty.");
    check(outputs.size() > 0, "outputs are empty.");

    // Generate checksum from processId
    std::string process_checksum_string = company + ";" + processId;
    checksum256 process_checksum = sha256(process_checksum_string.c_str(), process_checksum_string.size());

    // Access table and make sure process doesnt exist
    auto processes_bycompandid = _processes.get_index<eosio::name("bycompandid")>();
    auto process = processes_bycompandid.find(process_checksum);
    check(process == processes_bycompandid.end(), "process already exists");

    // Create new process
    _processes.emplace(get_self(), [&](auto& b) {
        b.index     = _processes.available_primary_key();
        b.createdBy = user;
        b.updatedBy = user;
        b.createdAt = timestamp;
        b.updatedAt = timestamp;

        b.company   = company;
        b.processId = processId;
        b.type      = type;
        b.site      = site;
        b.startTime = startTime;

        b.inputs    = inputs;
        b.outputs   = outputs;

        // Optional
        if (endTime)     b.endTime     = *endTime;
        if (machine)     b.machine     = *machine;
        if (status)      b.status      = *status;
        if (description) b.description = *description;
        if (version)     b.version     = *version;

        // Process immediately
        if (type == ProcessType::SPLIT || type == ProcessType::MERGE) {
            b.status = ProcessStatus::PROCESSED;

            if (!endTime) {
                b.endTime = timestamp;
            }
        }

        // New process, so we substract inputs from our site
        std::map<std::string, ProductQuantity> emptyDeltas;
        processcargo(b, b.inputs, emptyDeltas, user, company, Actions::NEW_PROCESS, false);
    });
}

/**
 * Edit Process
 **/
void tracelytics::editprocess (
    const std::string& user,
    const std::string& company,
    const std::string& processId,
    const std::map<std::string, ProductQuantity>& inputDeltas,
    const std::map<std::string, ProductQuantity>& outputDeltas,
    const time_point& timestamp,
    const std::map<std::string, all_type>& data,

    const optional<time_point>& startTime,
    const optional<time_point>& endTime,
    const optional<std::string>& machine,
    const optional<std::string>& status,
    const optional<std::string>& description,
    const optional<std::string>& version
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),      "user is missing.");
    check(!company.empty(),   "company is missing.");
    check(!processId.empty(), "process id is missing.");

    // Generate checksum from processId
    std::string process_checksum_string = company + ";" + processId;
    checksum256 process_checksum = sha256(process_checksum_string.c_str(), process_checksum_string.size());

    // Access table and make sure process doesnt exist
    auto processes_bycompandid = _processes.get_index<eosio::name("bycompandid")>();
    auto process = processes_bycompandid.find(process_checksum);
    check(process != processes_bycompandid.end(), "process does not exist.");
    check(company == process->company && processId == process->processId, "process mismatch");

    // Edit process
    processes_bycompandid.modify(process, get_self(), [&](auto& b) {
        b.updatedBy = user;
        b.updatedAt = timestamp;

        // Checks: 1) New status is PROCESSED and old status is not PROCESSED
        bool justProcessed = status.value_or("") == ProcessStatus::PROCESSED && b.status != ProcessStatus::PROCESSED;

        // Optional
        if (startTime)   b.startTime   = *startTime;
        if (endTime)     b.endTime     = *endTime;
        if (machine)     b.machine     = *machine;
        if (status)      b.status      = *status;
        if (description) b.description = *description;
        if (version)     b.version     = *version;

        // Edit deltas
        if (b.status != ProcessStatus::PROCESSED) {
          if (inputDeltas.size() > 0)
            processcargo(b, b.inputs, inputDeltas, user, company, Actions::EDIT_PROCESS, false);
          if (outputDeltas.size() > 0)
            processcargo(b, b.outputs, outputDeltas, user, company, Actions::EDIT_PROCESS, true);
        }

        // Update inventory if processed
        if (justProcessed) {
          std::map<std::string, ProductQuantity> emptyDeltas;
          processcargo(b, b.outputs, emptyDeltas, user, company, Actions::EDIT_PROCESS, false);
        }
    });
}

/**
 * Delete Process
 **/
void tracelytics::delprocess (
    const std::string& user,
    const std::string& company,
    const std::string& processId,
    const time_point&  timestamp
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),      "user is missing.");
    check(!company.empty(),   "company is missing.");
    check(!processId.empty(), "process id is missing.");

    // Generate checksum from processId
    std::string process_checksum_string = company + ";" + processId;
    checksum256 process_checksum = sha256(process_checksum_string.c_str(), process_checksum_string.size());

    // Access table and make sure process doesnt exist
    auto processes_bycompandid = _processes.get_index<eosio::name("bycompandid")>();
    auto process = processes_bycompandid.find(process_checksum);
    check(process != processes_bycompandid.end(), "process does not exist.");
    check(company == process->company && processId == process->processId, "process mismatch");

    // Delete process
    processes_bycompandid.erase(process);
}
