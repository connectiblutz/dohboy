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
    {
      runSocket();
      break;
    }
    case Worker::MSG_ONDNSPACKET:
    {
      auto data = message.data<DnsPacketData>();
      handleDnsPacket(data.get());
      break;
    }
    default:
      break;
  }
}

void Worker::OnStop() {
  bcl::LogUtil::Debug()<<"closing socket";
  socket->close();
}

void Worker::handleDnsPacket(DnsPacketData* data) {
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
  bool isSingleThread = (std::thread::hardware_concurrency()==1);
  auto addr = bcl::SocketAddress("0.0.0.0",53);
  socket = std::make_shared<bcl::UdpServerSocket>(addr);
  auto buffer = std::shared_ptr<char>(new char[4096], std::default_delete<char[]>());
  while (socket->isListening()) {
    bcl::SocketAddress source("",0);
    uint16_t bytes = socket->ReadPacket(source,buffer.get(), 4096);
    if (bytes>0) {
      if (isSingleThread) {
        auto packetdata = DnsPacketData(source,buffer,bytes);
        handleDnsPacket(&packetdata);
      } else {
        post(Message(Worker::MSG_ONDNSPACKET,std::make_shared<DnsPacketData>(source,buffer,bytes)));
        buffer = std::shared_ptr<char>(new char[4096], std::default_delete<char[]>());
      }
    }
  }
}

}