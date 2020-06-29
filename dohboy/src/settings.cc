#include "settings.h"
#include <bcl/fileutil.h>

namespace dohboy {


Settings Settings::instance;

void Settings::Set(const std::filesystem::path& file) {
  std::string configStr = bcl::FileUtil::FileToString(file);
  cJSON* configJson = cJSON_Parse(configStr.c_str());

  if (cJSON_HasObjectItem(configJson, "bindAddress")) {
    cJSON* bind = cJSON_GetObjectItem(configJson, "bindAddress");
    if (bind && cJSON_IsString(bind)) {
      instance.bindAddress=bcl::SocketAddress(cJSON_GetStringValue(bind));
    }
  }
  
  if (cJSON_HasObjectItem(configJson, "domain")) {
    cJSON* domain = cJSON_GetObjectItem(configJson, "domain");
    if (domain && cJSON_IsString(domain)) {
      instance.domain=cJSON_GetStringValue(domain);
    }
  }

  if (cJSON_HasObjectItem(configJson, "domainIP")) {
    cJSON* domainIP = cJSON_GetObjectItem(configJson, "domainIP");
    if (domainIP && cJSON_IsString(domainIP)) {
      instance.domainIP=bcl::IPAddress(cJSON_GetStringValue(domainIP));
    }
  }
  
  if (cJSON_HasObjectItem(configJson, "forceSingleThread")) {
    cJSON* forceSingleThread = cJSON_GetObjectItem(configJson, "forceSingleThread");
    if (forceSingleThread && cJSON_IsBool(forceSingleThread)) {
      instance.forceSingleThread=cJSON_IsTrue(forceSingleThread);
    }
  }

  cJSON_free(configJson);
}

}