#pragma once

#include <string>
#include <memory>

namespace dns2doh {

struct DnsPacketData {
  DnsPacketData(std::string source, std::shared_ptr<char> data, uint16_t size) : source(source), data(data), size(size) {}
  std::string source;
  std::shared_ptr<char> data;
  uint16_t size;
};

}