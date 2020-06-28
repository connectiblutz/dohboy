#include "worker.h"
#include <bcl/logutil.h>
#include <message.h>
#include "doh.h"
#include <bcl/socketaddress.h>
#include <curl/curl.h>

namespace dns2doh {

const uint16_t Worker::MSG_RUNSOCKET = 1;
const uint16_t Worker::MSG_ONDNSPACKET = 2;

Worker::Worker(std::string config) : _config(config) {
  initConfig();
  curl_global_init(CURL_GLOBAL_ALL);
  post(MSG_RUNSOCKET);
}

Worker::~Worker() {

}

void Worker::initConfig() {

}


void Worker::OnMessage(Message message) {
  switch (message.code()) {
    case Worker::MSG_RUNSOCKET:
      runSocket();
      break;
    case Worker::MSG_ONDNSPACKET:
      handleDnsPacket(message.data<DnsPacketData>());
      break;
    default:
      break;
  }
}

void Worker::handleDnsPacket(std::shared_ptr<DnsPacketData> data) {
  auto dnsPacket = dns::Message();
  dnsPacket.decode(data->data.get(),data->size);
  if (dnsPacket.getQdCount()) {
    for (auto query : dnsPacket.getQueries()) {
      bcl::LogUtil::Debug() << "have query for "<<query->getName()<< " from "<<data->source.toString();
    }
    DoH::Lookup(dnsPacket);
    dns::uint size;
    dnsPacket.encode(data->data.get(),4096,size);
    socket->WritePacket(data->source,data->data.get(),size);
  }
}

void Worker::runSocket() {
  auto addr = bcl::SocketAddress("0.0.0.0",53);
  socket = std::make_shared<bcl::UdpServerSocket>(addr);
  while (socket->isListening()) {
    socket->ReadPacket([this](const bcl::SocketAddress& source,std::shared_ptr<char> data, uint16_t size) {
      auto packetdata = std::make_shared<DnsPacketData>(source,data,size);
      post(Message(Worker::MSG_ONDNSPACKET,packetdata));
    },4096);
  }
}

}