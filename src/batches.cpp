#include "tracelytics/tracelytics.hpp"

/**
 * Create New Batch
 *
 * Params:
 *  - company (name)
 *  - batchId (string)
 *  - userId (string) (optional)
 *  - machineId (string) (optional)
 *  - date (string) (optional)
 **/
void tracelytics::newbatch (std::map<std::string, all_type> args) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company    =  std::get_if<name>       (  &args["company"]   );
    auto batchId    =  std::get_if<std::string>(  &args["batchId"]   );
    auto userId     =  std::get_if<std::string>(  &args["userId"]    ); // Optional
    auto machineId  =  std::get_if<std::string>(  &args["machineId"] ); // Optional
    auto date       =  std::get_if<std::string>(  &args["date"]      ); // Optional

    // Validation
    check(company != nullptr, "company is missing.");
    check(batchId != nullptr, "batch id is missing.");

    // Generate checksum from batchId
    checksum256 batch_checksum = sha256(batchId->c_str(), batchId->size());

    // Access table and make sure batch doesnt exist
    batch_table batches(get_self(), company->value);
    auto batches_bychecksum = batches.get_index<"bychecksum"_n>();
    auto batch = batches_bychecksum.find(batch_checksum);
    check(batch == batches_bychecksum.end(), "batch already exists");

    // Create new batch
    batches.emplace(get_self(), [&](auto& b) {
        b.id = batches.available_primary_key();
        b.batch_checksum = batch_checksum;
        b.batch_id = *batchId;

        // Optional
        if (userId    != nullptr) b.user_id    = *userId;
        if (machineId != nullptr) b.machine_id = *machineId;
        if (date      != nullptr) b.date       = *date;
    });
}

/**
 * Edit Batch
 *
 * Params:
 *  - company (name)
 *  - batchId (string)
 *  - userId (string) (optional)
 *  - machineId (string) (optional)
 *  - date (string) (optional)
 **/
void tracelytics::editbatch (std::map<std::string, all_type> args) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company    =  std::get_if<name>       (  &args["company"]   );
    auto batchId    =  std::get_if<std::string>(  &args["batchId"]   );
    auto userId     =  std::get_if<std::string>(  &args["userId"]    ); // Optional
    auto machineId  =  std::get_if<std::string>(  &args["machineId"] ); // Optional
    auto date       =  std::get_if<std::string>(  &args["date"]      ); // Optional

    // Validation
    check(company != nullptr, "company is missing.");
    check(batchId != nullptr, "batch id is missing.");

    // Generate checksum from batchId
    checksum256 batch_checksum = sha256(batchId->c_str(), batchId->size());

    // Access table and make sure batch doesnt exist
    batch_table batches(get_self(), company->value);
    auto batches_bychecksum = batches.get_index<"bychecksum"_n>();
    auto batch = batches_bychecksum.find(batch_checksum);
    check(batch != batches_bychecksum.end(), "batch does not exist.");

    // Edit batch
    batches_bychecksum.modify(batch, get_self(), [&](auto& b) {
        // Optional
        if (userId    != nullptr) b.user_id    = *userId;
        if (machineId != nullptr) b.machine_id = *machineId;
        if (date      != nullptr) b.date       = *date;
    });

    // TODO Create adjustments table for logging maybe>?
}

/**
 * Delete Batch
 *
 * Params:
 *  - company (name)
 *  - batchId (string)
 **/
void tracelytics::delbatch (std::map<std::string, all_type> args) {
    // Authentication
    require_auth( get_self() );

    // Args
    auto company = std::get_if<name>       (  &args["company"] );
    auto batchId = std::get_if<std::string>(  &args["batchId"] );

    // Validation
    check(company != nullptr, "company is missing.");
    check(batchId != nullptr, "batch id is missing.");

    // Generate checksum from batchId
    checksum256 batch_checksum = sha256(batchId->c_str(), batchId->size());

    // Access table and make sure batch doesnt exist
    batch_table batches(get_self(), company->value);
    auto batches_bychecksum = batches.get_index<"bychecksum"_n>();
    auto batch = batches_bychecksum.find(batch_checksum);
    check(batch != batches_bychecksum.end(), "batch does not exist.");

    // Delete batch
    batches_bychecksum.erase(batch);
}
