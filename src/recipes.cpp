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
    check(!recipeId.empty(),  "recipe ID is missing.");
    check(inputs.size() > 0,  "inputs are empty.");
    check(outputs.size() > 0, "outputs are empty.");

    // Access table and make sure recipe doesnt exist
    auto recipes_bycompandid = _recipes.get_index<eosio::name("bycompandid")>();
    auto recipe = recipes_bycompandid.find(Checksum::RECIPE(company, recipeId));
    check(recipe == recipes_bycompandid.end(), "recipe already exists");

    // Create new recipe
    _recipes.emplace(get_self(), [&](auto& r) {
        r.index     = _recipes.available_primary_key();
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
    check(!recipeId.empty(),  "recipe ID is missing.");
    check(inputs.size() > 0,  "inputs are empty.");
    check(outputs.size() > 0, "outputs are empty.");

    // Access table and make sure recipe exists
    auto recipes_bycompandid = _recipes.get_index<eosio::name("bycompandid")>();
    auto recipe = recipes_bycompandid.find(Checksum::RECIPE(company, recipeId));
    check(recipe != recipes_bycompandid.end(), "recipe does not exist.");
    check(company == recipe->company && recipeId == recipe->recipeId, "recipe mismatch");

    // Edit recipe
    recipes_bycompandid.modify(recipe, get_self(), [&](auto& r) {
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
    check(!recipeId.empty(), "recipe ID is missing.");

    // Access table and make sure recipe exists
    auto recipes_bycompandid = _recipes.get_index<eosio::name("bycompandid")>();
    auto recipe = recipes_bycompandid.find(Checksum::RECIPE(company, recipeId));
    check(recipe != recipes_bycompandid.end(), "recipe does not exist.");
    check(recipeId == recipe->recipeId, "recipe mismatch");

    if (user != ADMIN) {
        check(company == recipe->company, "only employees of " + recipe->company + " can delete the recipe.");
    }

    // Delete recipe
    recipes_bycompandid.erase(recipe);
}
