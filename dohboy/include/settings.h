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
    static void Set(const std::filesystem::path& file);
    static const bcl::SocketAddress& getBindAddress() { return instance.bindAddress; }
    static const std::string& getDomain() { return instance.domain; }
    static const bcl::IPAddress& getDomainIP() { return instance.domainIP; }
    static bool getForceSingleThread() { return instance.forceSingleThread; }
    static const std::string& getLocalDomain() { return instance.localDomain; }
    static const std::string& getDhcpLeases() { return instance.dhcpLeases; }
  private:  
    static void load(std::string& target, cJSON* config, const std::string& name);
    static void load(bool& target, cJSON* config, const std::string& name);
    static void load(bcl::SocketAddress& target, cJSON* config, const std::string& name);
    static void load(bcl::IPAddress& target, cJSON* config, const std::string& name);
    static Settings instance;
    Settings() : bindAddress("0.0.0.0",53), domain("dns.quad9.net"), domainIP("9.9.9.9"), forceSingleThread(false), localDomain(".local"), dhcpLeases("/var/lib/dhcp/dhcpd.leases") { };
    bcl::SocketAddress bindAddress;
    std::string domain;
    bcl::IPAddress domainIP;
    bool forceSingleThread;
    std::string localDomain;
    std::string dhcpLeases;


};

}