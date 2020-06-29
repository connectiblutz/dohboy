#pragma once

#include <bcl/socketaddress.h>
#include <memory>

namespace dohboy {

struct DnsPacketData {
  DnsPacketData(const bcl::SocketAddress& source, std::shared_ptr<char> data, uint16_t size) : source(source), data(data), size(size) {}
  bcl::SocketAddress source;
  std::shared_ptr<char> data;
  uint16_t size;
};

}