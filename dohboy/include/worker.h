#pragma once

#include <bcl/messagethreadpool.h>
#include <string>
#include <bcl/serversocketutil.h>
#include "dnspacketdata.h"

namespace dohboy {

class Worker : public bcl::MessageThreadPool {
  public:
    Worker();
    ~Worker();
  protected:
    virtual void OnMessage(Message message);
    virtual void OnStop();
  private:
    std::string _config;
    std::shared_ptr<bcl::UdpServerSocket> socket;
    void runSocket();
    void handleDnsPacket(DnsPacketData* data);
    static const uint16_t MSG_RUNSOCKET;
    static const uint16_t MSG_ONDNSPACKET;

};

}