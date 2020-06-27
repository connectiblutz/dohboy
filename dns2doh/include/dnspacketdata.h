#pragma once

#include <string>
#include <memory>

namespace dns2doh {

struct DnsPacketData {
  DnsPacketData(std::string source, std::shared_ptr<char> data) : source(source), data(data) {}
  std::string source;
  std::shared_ptr<char> data;
};

}