#include "tracelytics/tracelytics.hpp"

void tracelytics::cleartable (const std::string& tableName) {
  require_auth(get_self());

  if (tableName == "inventorylog") {
    cleanTable<inventory_log_table>();
  } else if (tableName == "item") {
    cleanTable<item_table>();
  } else if (tableName == "user") {
    cleanTable<user_table>();
  } else if (tableName == "site") {
    cleanTable<site_table>();
  }
}

void tracelytics::clearall () {
  require_auth(get_self());
  cleanTable<inventory_log_table>();
  cleanTable<item_table>();
  cleanTable<user_table>();
  cleanTable<site_table>();
}

void tracelytics::ec_verify(std::string data, const signature &sig, const public_key &pk) {
  checksum256 digest = eosio::sha256(data.c_str(), data.size());
  eosio::assert_recover_key(digest, sig, pk);
  // print("VALID ECVERIFY");
}

void tracelytics::verify_auth(std::string company, std::string userId, std::string entity, std::string action, std::string verifydata) {
    auto users_byid = _users.get_index<eosio::name("byid")>();
    auto user = users_byid.find(Checksum::USER(userId));
    check( user != users_byid.end(), "user does not exist" );
    check( user->nonce == std::stoull(verifydata), "incorrect nonce" );
    check( std::count(user->permissions.begin(), user->permissions.end(), entity + ":" + action), "invalid permissions");
}

std::vector<std::string> tracelytics::split(std::string str, std::string token){
  std::vector<std::string> result;
  while (str.size()) {
    int index = str.find(token);
    if (index != std::string::npos) {
      result.push_back(str.substr(0,index));
      str = str.substr(index + token.size());
      if (str.size() == 0) {
        result.push_back(str);
      }
    } else {
      result.push_back(str);
      str = "";
    }
  }
  return result;
}

std::string tracelytics::to_hex(const char* d, uint32_t s) {
  std::string r;
  const char* to_hex = "0123456789abcdef";
  uint64_t* c = (uint64_t*)d;
  for (uint32_t i = 0; i < s; ++i) {
    (r += to_hex[(c[i] >> 4)]) += to_hex[(c[i] & 0x0f)];
  }
  return r;
}

uint64_t tracelytics::truncate_sha256_to_uint64(const checksum256& sha256) {
  auto array = sha256.extract_as_byte_array();
  uint64_t value =
    static_cast<uint64_t>(array[0]) |
    static_cast<uint64_t>(array[1]) << 8 |
    static_cast<uint64_t>(array[2]) << 16 |
    static_cast<uint64_t>(array[3]) << 24 |
    static_cast<uint64_t>(array[4]) << 32 |
    static_cast<uint64_t>(array[5]) << 40 |
    static_cast<uint64_t>(array[6]) << 48 |
    static_cast<uint64_t>(array[7]) << 56;

  return value;
}

checksum256 tracelytics::SHA256(const std::string& stringToSha) {
  return sha256(stringToSha.c_str(), stringToSha.size());
}
