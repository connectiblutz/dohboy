#include "settings.h"
#include <bcl/fileutil.h>

namespace dohboy {


Settings Settings::instance;


void Settings::load(std::string& target, cJSON* config, const std::string& name) {  
  if (cJSON_HasObjectItem(config, name.c_str())) {
    cJSON* tmp = cJSON_GetObjectItem(config, name.c_str());
    if (tmp && cJSON_IsString(tmp)) {
      target=cJSON_GetStringValue(tmp);
    }
  }
}
void Settings::load(bool& target, cJSON* config, const std::string& name) {  
  if (cJSON_HasObjectItem(config, name.c_str())) {
    cJSON* tmp = cJSON_GetObjectItem(config, name.c_str());
    if (tmp && cJSON_IsBool(tmp)) {
      target=cJSON_IsTrue(tmp);
    }
  }
}
void Settings::load(bcl::SocketAddress& target, cJSON* config, const std::string& name) {  
  if (cJSON_HasObjectItem(config, name.c_str())) {
    cJSON* tmp = cJSON_GetObjectItem(config, name.c_str());
    if (tmp && cJSON_IsString(tmp)) {
      target=bcl::SocketAddress(cJSON_GetStringValue(tmp));
    }
  }
}
void Settings::load(bcl::IPAddress& target, cJSON* config, const std::string& name) {  
  if (cJSON_HasObjectItem(config, name.c_str())) {
    cJSON* tmp = cJSON_GetObjectItem(config, name.c_str());
    if (tmp && cJSON_IsString(tmp)) {
      target=bcl::IPAddress(cJSON_GetStringValue(tmp));
    }
  }
}

void Settings::Set(const std::filesystem::path& file) {
  (void)(file);
  std::string configStr = bcl::FileUtil::FileToString(file);
  cJSON* configJson = cJSON_Parse(configStr.c_str());

  Settings::load(instance.bindAddress,configJson,"bindAddress");
  Settings::load(instance.domain,configJson,"domain");
  Settings::load(instance.domainIP,configJson,"bindAddress");
  Settings::load(instance.forceSingleThread,configJson,"forceSingleThread");
  Settings::load(instance.localDomain,configJson,"localDomain");
  Settings::load(instance.dhcpLeases,configJson,"dhcpLeases");

  cJSON_free(configJson);
}

}