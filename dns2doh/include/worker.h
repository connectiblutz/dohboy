#pragma once

#include <bcl/messagethreadpool.h>
#include <string>
#include <bcl/serversocketutil.h>
#include "dnspacketdata.h"

namespace dns2doh {

class Worker : public bcl::MessageThreadPool {
  public:
    Worker(std::string config = "");
    ~Worker();
  protected:
    virtual void OnMessage(Message message);
  private:
    std::string _config;
    std::shared_ptr<bcl::UdpServerSocket> socket;
    void initConfig();
    void runSocket();
    void handleDnsPacket(std::shared_ptr<DnsPacketData> data);
    static const uint16_t MSG_RUNSOCKET;
    static const uint16_t MSG_ONDNSPACKET;

};

}