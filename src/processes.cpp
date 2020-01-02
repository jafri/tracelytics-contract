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
    process_table processes(get_self(), get_self().value);
    auto processes_bychecksum = processes.get_index<"bychecksum"_n>();
    auto process = processes_bychecksum.find(process_checksum);
    check(process == processes_bychecksum.end(), "process already exists");

    // Create new process
    processes.emplace(get_self(), [&](auto& b) {
        b.index     = processes.available_primary_key();
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
    });

    // Substract quantity for each item from inventory
    for( auto const& [item, productAndQuantity] : inputs ) {
        // Process item quantity change
        upsertitem(user,
                   company,
                   site,
                   item,
                   productAndQuantity.product,
                  -productAndQuantity.quantity, // NEGATIVE
                   productAndQuantity.metadata,
                   (std::string) "newprocess",
                   processId,
                   timestamp);
    }

    // Split or merge type
    // Finishes processing the process immediately
    if (type == ProcessType::Split || type == ProcessType::Merge) {
        std::map<std::string, ProductQuantity> call_inputs;
        std::map<std::string, ProductQuantity> call_outputs;
        std::map<std::string, all_type> call_data;
        tracelytics::editprocess_action editprocess_action( get_self(), {get_self(), "active"_n} );
        editprocess_action.send(user,
                                company,
                                processId,
                                call_inputs,
                                call_outputs,
                                timestamp,
                                call_data,

                                std::nullopt,
                                std::nullopt,
                                std::nullopt,
                                (std::string) "processed",
                                std::nullopt,
                                std::nullopt);
    }
}

/**
 * Edit Process
 **/
void tracelytics::editprocess (
    const std::string& user,
    const std::string& company,
    const std::string& processId,
    const std::map<std::string, ProductQuantity>& inputsDelta,
    const std::map<std::string, ProductQuantity>& outputsDelta,
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
    process_table processes(get_self(), get_self().value);
    auto processes_bychecksum = processes.get_index<"bychecksum"_n>();
    auto process = processes_bychecksum.find(process_checksum);
    check(process != processes_bychecksum.end(), "process does not exist.");

    // Update inventory if processed
    if (process->status != "processed" && status.value_or("") == "processed") {
        // For each cargo
        for( auto const& [item, productAndQuantity] : process->outputs ) {
            // Process item quantity change
            upsertitem(user,
                       company,
                       process->site,
                       item,
                       productAndQuantity.product,
                       productAndQuantity.quantity,
                       productAndQuantity.metadata,
                       (std::string) "editprocess",
                       processId,
                       timestamp);
        }
    }

    // Edit process
    processes_bychecksum.modify(process, get_self(), [&](auto& b) {
        b.updatedBy = user;
        b.updatedAt = timestamp;

        // Optional
        if (startTime)   b.startTime   = *startTime;
        if (endTime)     b.endTime     = *endTime;
        if (machine)     b.machine     = *machine;
        if (status)      b.status      = *status;
        if (description) b.description = *description;
        if (version)     b.version     = *version;

        // Process input deltas
        if (inputsDelta.size() > 0) {
            // Consistent
            auto* mapOfItems = &b.inputs;
            auto* mapOfDeltas = &inputsDelta;
            auto* row = &b;

            for( auto const& [item, productAndQuantity] : *mapOfDeltas ) {
                // Ensure new quantity is zero or positive
                check(productAndQuantity.quantity >= 0, "quantity in delivery must be zero or positive for item " + item);

                // processingDelta = new quantity of item - current quantity
                int64_t processingDelta = mapOfItems->count(item) > 0
                    ? productAndQuantity.quantity - (*mapOfItems)[item].quantity // In cargo
                    : productAndQuantity.quantity;                               // Not in cargo
                int64_t inventoryDelta = -processingDelta;

                /**
                 * inventoryDelta has 3 states
                 * Zero: No change
                 * Positive (+): - Cargo -> + Inventory (refund)
                 * Negative (-): - Inventory -> + Cargo (charge)
                 **/

                // Skip if processingDelta is 0, as nothing changes
                if (inventoryDelta == 0) {
                    continue;
                }

                // Update cargo
                if (productAndQuantity.quantity > 0) {
                    (*mapOfItems)[item] = productAndQuantity;
                } else if (productAndQuantity.quantity == 0) {
                    (*mapOfItems).erase(item);
                }

                // Process item quantity change
                upsertitem(user,
                           company,
                           row->site,
                           item,
                           productAndQuantity.product,
                           inventoryDelta,
                           productAndQuantity.metadata,
                           (std::string) "editprocess",
                           processId,
                           timestamp);
            }
        }

        // Process output deltas
        if (outputsDelta.size() > 0) {
            // Consistent
            auto* mapOfItems = &b.outputs;
            auto* mapOfDeltas = &outputsDelta;
            auto* row = &b;

            // Initialize items table
            item_table items(get_self(), get_self().value);
            auto items_bychecksum = items.get_index<"bysiteitem"_n>();

            for( auto const& [item, productAndQuantity] : *mapOfDeltas ) {
                // Ensure new quantity is zero or positive
                check(productAndQuantity.quantity >= 0, "quantity in delivery must be zero or positive for item " + item);

                // processingDelta = new quantity of item - current quantity
                int64_t processingDelta = mapOfItems->count(item) > 0
                    ? productAndQuantity.quantity - (*mapOfItems)[item].quantity // In cargo
                    : productAndQuantity.quantity;                               // Not in cargo
                int64_t inventoryDelta = -processingDelta;

                /**
                 * inventoryDelta has 3 states
                 * Zero: No change
                 * Positive (+): - Cargo -> + Inventory (refund)
                 * Negative (-): - Inventory -> + Cargo (charge)
                 **/

                // Skip if processingDelta is 0, as nothing changes
                if (inventoryDelta == 0) {
                    continue;
                }

                // Update cargo
                if (productAndQuantity.quantity > 0) {
                    (*mapOfItems)[item] = productAndQuantity;
                } else if (productAndQuantity.quantity == 0) {
                    (*mapOfItems).erase(item);
                }
            }
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
    process_table processes(get_self(), get_self().value);
    auto processes_bychecksum = processes.get_index<"bychecksum"_n>();
    auto process = processes_bychecksum.find(process_checksum);
    check(process != processes_bychecksum.end(), "process does not exist.");

    // Delete process
    processes_bychecksum.erase(process);
}
