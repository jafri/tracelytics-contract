#include "tracelytics/tracelytics.hpp"

void tracelytics::ec_verify(std::string data, const signature &sig, const public_key &pk) {
    checksum256 digest = eosio::sha256(data.c_str(), data.size());
    eosio::assert_recover_key(digest, sig, pk);
    print("VALID ECVERIFY");
}

void tracelytics::verify_auth(name company, name username, std::string entity, std::string action, std::string verifydata) {
    user_table users(get_self(), company.value);
    auto user = users.find(username.value);
    check( user != users.end(), "user does not exist" );
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
  uint8_t* c = (uint8_t*)d;
  for (uint32_t i = 0; i < s; ++i) {
    (r += to_hex[(c[i] >> 4)]) += to_hex[(c[i] & 0x0f)];
  }
  return r;
}
