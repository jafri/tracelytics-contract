#include "tracelytics/tracelytics.hpp"

/**
 * Create New recipe
 **/
void tracelytics::newrecipe (
    const std::string& user,
    const std::string& company,
    const std::string& recipeId,
    const std::vector<ProductQuantity>& inputs,
    const std::vector<ProductQuantity>& outputs,
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
    check(!recipeId.empty(),  "recipe id is missing.");
    check(inputs.size() > 0,  "inputs are empty.");
    check(outputs.size() > 0, "outputs are empty.");

    // Generate checksum from recipeId
    std::string recipe_checksum_string = company + ";" + recipeId;
    checksum256 recipe_checksum = sha256(recipe_checksum_string.c_str(), recipe_checksum_string.size());

    // Access table and make sure recipe doesnt exist
    recipe_table recipes(get_self(), get_self().value);
    auto recipes_bychecksum = recipes.get_index<"bychecksum"_n>();
    auto recipe = recipes_bychecksum.find(recipe_checksum);
    check(recipe == recipes_bychecksum.end(), "recipe already exists");

    // Create new recipe
    recipes.emplace(get_self(), [&](auto& r) {
        r.index     = recipes.available_primary_key();
        r.createdBy = user;
        r.updatedBy = user;
        r.createdAt = timestamp;
        r.updatedAt = timestamp;

        r.company   = company;
        r.recipeId  = recipeId;
        r.inputs    = inputs;
        r.outputs   = outputs;

        // Optional
        if (name)        r.name        = *name;
        if (description) r.description = *description;
        if (version)     r.version     = *version;
    });
}

/**
 * Edit recipe
 **/
void tracelytics::editrecipe (
    const std::string& user,
    const std::string& company,
    const std::string& recipeId,
    const std::vector<ProductQuantity>& inputs,
    const std::vector<ProductQuantity>& outputs,
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
    check(!recipeId.empty(),  "recipe id is missing.");
    check(inputs.size() > 0,  "inputs are empty.");
    check(outputs.size() > 0, "outputs are empty.");

    // Generate checksum from recipeId
    std::string recipe_checksum_string = company + ";" + recipeId;
    checksum256 recipe_checksum = sha256(recipe_checksum_string.c_str(), recipe_checksum_string.size());

    // Access table and make sure recipe exists
    recipe_table recipes(get_self(), get_self().value);
    auto recipes_bychecksum = recipes.get_index<"bychecksum"_n>();
    auto recipe = recipes_bychecksum.find( recipe_checksum );
    check(recipe != recipes_bychecksum.end(), "recipe does not exist.");

    // Edit recipe
    recipes_bychecksum.modify(recipe, get_self(), [&](auto& r) {
        r.updatedBy = user;
        r.updatedAt = timestamp;

        r.inputs    = inputs;
        r.outputs   = outputs;

        // Optional
        if (name)        r.name        = *name;
        if (description) r.description = *description;
        if (version)     r.version     = *version;
    });
}

/**
 * Delete recipe
 **/
void tracelytics::delrecipe (
    const std::string& user,
    const std::string& company,
    const std::string& recipeId,
    const time_point&  timestamp
) {
    // Authentication
    require_auth( get_self() );

    // Validation
    check(!user.empty(),     "user is missing.");
    check(!company.empty(),  "company is missing.");
    check(!recipeId.empty(), "recipe id is missing.");

    // Generate checksum from recipeId
    std::string recipe_checksum_string = company + ";" + recipeId;
    checksum256 recipe_checksum = sha256(recipe_checksum_string.c_str(), recipe_checksum_string.size());

    // Access table and make sure recipe exists
    recipe_table recipes(get_self(), get_self().value);
    auto recipes_bychecksum = recipes.get_index<"bychecksum"_n>();
    auto recipe = recipes_bychecksum.find(recipe_checksum);
    check(recipe != recipes_bychecksum.end(), "recipe does not exist.");

    // Delete recipe
    recipes_bychecksum.erase(recipe);
}
