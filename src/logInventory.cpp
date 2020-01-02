#include "tracelytics/tracelytics.hpp"

void tracelytics::loginventory (
    const std::string& user,
    const std::string& company,
    const std::string& item,
    const std::string& site,
    const std::string& product,
    std::map<std::string, std::string> metadata,
    const std::string& action,
    const std::string& parentAction,
    const std::string& parentActionId,
    const time_point&  timestamp,
    const std::string& version,
    const int64_t&     oldQuantity,
    const int64_t&     newQuantity
) {
    require_auth( get_self() );

    // Log
    inventory_log_table inventory_logs(get_self(), get_self().value);
    inventory_logs.emplace(get_self(), [&](auto& i) {
        i.index          = inventory_logs.available_primary_key();
        i.user           = user;
        i.company        = company;
        i.item           = item;
        i.site           = site;
        i.product        = product;
        i.action         = action;
        i.parentAction   = parentAction;
        i.parentActionId = parentActionId;
        i.version        = version;
        i.metadata       = metadata;
        i.timestamp      = timestamp;

        i.oldQuantity    = oldQuantity;
        i.newQuantity    = newQuantity;
        i.delta          = newQuantity - oldQuantity;

        // TX ID
        auto size = transaction_size();
        char buf[size];
        uint32_t read = read_transaction( buf, size );
        check( size == read, "read_transaction failed");
        i.txid = sha256(buf, size);
    });
}
