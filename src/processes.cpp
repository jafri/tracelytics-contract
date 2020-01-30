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
    const std::map<std::string, std::string>& data,

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
    check(!processId.empty(), "process ID is missing.");
    check(!type.empty(),      "type is missing.");
    check(!site.empty(),      "site is missing.");
    check(inputs.size() > 0,  "inputs are empty.");

    // Check site exists
    check_site_exists(company, site);

    // Access table and make sure process doesnt exist
    auto processes_bycompandid = _processes.get_index<eosio::name("bycompandid")>();
    auto process = processes_bycompandid.find(Checksum::PROCESS(company, processId));
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

        // New process, so we substract inputs from our site
        std::map<std::string, ProductQuantity> emptyDeltas;
        processcargo(b, b.inputs, emptyDeltas, user, company, Actions::NEW_PROCESS, ProcessActivity::START_PROCESS);

        // Process immediately
        if (type == ProcessType::SPLIT || type == ProcessType::MERGE || type == ProcessType::SCRAP || type == ProcessType::ADJUSTMENT) {
            b.status = ProcessStatus::PROCESSED;
            if (!endTime) {
                b.endTime = timestamp;
            }
        }

        // Process immediately
        if (b.status == ProcessStatus::PROCESSED) {
            processcargo(b, b.outputs, emptyDeltas, user, company, Actions::NEW_PROCESS, ProcessActivity::FINISH_PROCESS);
        }
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
    const std::map<std::string, std::string>& data,

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
    check(!processId.empty(), "process ID is missing.");

    // Access table and make sure process doesnt exist
    auto processes_bycompandid = _processes.get_index<eosio::name("bycompandid")>();
    auto process = processes_bycompandid.find(Checksum::PROCESS(company, processId));
    check(process != processes_bycompandid.end(), "process does not exist.");
    check(processId == process->processId, "process mismatch");

    if (user != ADMIN) {
        check(company == process->company, "only employees of " + process->company + " can edit the process.");
        check(process->status != ProcessStatus::PROCESSED, "cannot edit a processed process");
        check(process->status != ProcessStatus::CANCELLED, "cannot edit a cancelled process");
    }

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
        if (inputDeltas.size() > 0) {
            processcargo(b, b.inputs, inputDeltas, user, company, Actions::EDIT_PROCESS, ProcessActivity::EDIT_INPUTS);
        }
        if (outputDeltas.size() > 0) {
            processcargo(b, b.outputs, outputDeltas, user, company, Actions::EDIT_PROCESS, ProcessActivity::EDIT_OUTPUTS);
        }

        // Update inventory if processed
        if (justProcessed) {
          std::map<std::string, ProductQuantity> emptyDeltas;
          processcargo(b, b.outputs, emptyDeltas, user, company, Actions::EDIT_PROCESS, ProcessActivity::FINISH_PROCESS);
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
    const time_point&  timestamp,
    const bool& cancel
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),      "user is missing.");
    check(!company.empty(),   "company is missing.");
    check(!processId.empty(), "process ID is missing.");

    // Access table and make sure process doesnt exist
    auto processes_bycompandid = _processes.get_index<eosio::name("bycompandid")>();
    auto process = processes_bycompandid.find(Checksum::PROCESS(company, processId));
    check(process != processes_bycompandid.end(), "process does not exist.");
    check(processId == process->processId, "process mismatch");

    if (user != ADMIN) {
        check(company == process->company, "only employees of " + process->company + " can " +
                                        (cancel ? "cancel" : "delete") + " the process.");
        check(process->status != ProcessStatus::PROCESSED, "cannot cancel or delete a processed process");
        check(process->status != ProcessStatus::CANCELLED, "cannot cancel or delete a cancelled process");
    }

    if (cancel) {
        processes_bycompandid.modify(process, get_self(), [&](auto& b) {
            b.updatedBy = user;
            b.updatedAt = timestamp;
            b.status = ProcessStatus::CANCELLED;

            // Refund
            std::map<std::string, ProductQuantity> emptyDeltas;
            processcargo(b, b.inputs, emptyDeltas, user, company, Actions::EDIT_PROCESS, ProcessActivity::FINISH_PROCESS);
        });
    } else {
        processes_bycompandid.erase(process);
    }
}
