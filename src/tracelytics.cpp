#include "tracelytics/tracelytics.hpp"

void tracelytics::push(
    std::string verifydata,
    const signature &sig,
    const public_key &pk,
    std::string username,
    std::string company,
    std::string entity,
    std::string action,
    std::map<std::string, std::string> data
) {
    verify_auth(company, username, entity, action, verifydata);
    ec_verify(verifydata, sig, pk);

    if (action == "create") {
        // tracelytics::create_action c_action( get_self(), {get_self(), eosio::name("active")} );
        // c_action.send(username, company, entity, args);
    } else if (action == "patch") {
        // tracelytics::patch_action p_action( get_self(), {get_self(), eosio::name("active")} );
        // p_action.send(username, company, entity, args);
    } else if (action == "remove") {
        // tracelytics::remove_action r_action( get_self(), {get_self(), eosio::name("active")} );
        // r_action.send(username, company, entity, args);
    }
}