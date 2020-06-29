#pragma once

#include <cJSON.h>
#include <filesystem>
#include <string>
#include <bcl/ipaddress.h>
#include <bcl/socketaddress.h>

namespace dohboy {

class Settings {
  public:
    static Settings& Get() { return instance; };
    static void Set(std::filesystem::path file);
    static const bcl::SocketAddress& getBindAddress() { return instance.bindAddress; }
    static const std::string& getDomain() { return instance.domain; }
    static const bcl::IPAddress& getDomainIP() { return instance.domainIP; }
    static bool getForceSingleThread() { return instance.forceSingleThread; }
  private:
    static Settings instance;
    Settings() : bindAddress("0.0.0.0",53), domain("dns.quad9.net"), domainIP("9.9.9.9"), forceSingleThread(false) { };
    bcl::SocketAddress bindAddress;
    std::string domain;
    bcl::IPAddress domainIP;
    bool forceSingleThread;


};

}