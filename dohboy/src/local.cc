#include "local.h"
#include "settings.h"
#include <bcl/stringutil.h>
#include <bcl/fileutil.h>
#include <bcl/logutil.h>

namespace dohboy {

bool Local::Lookup(dns::Message& message) {
  static auto localDomain = Settings::getLocalDomain();
  if (localDomain.empty()) return false;

  uint queryType = dns::RDATA_ANY;
  auto queryClass = dns::QCLASS_ASTERISK;
  auto host = std::string();
  bool isLocal = false;
  for (auto query : message.getQueries()) {
    auto name = query->getName();
    if (bcl::StringUtil::endsWith(name,localDomain)) {
      isLocal=true;
      host = name.substr(0,name.length()-localDomain.length());
      queryType=query->getType();
      queryClass=query->getClass();
    }
  }

  if (!isLocal) return false;

  auto matchedIp = std::string();
  auto lastIp = std::string();
  static auto leaseNeedleStart = std::string("lease ");
  static auto leaseNeedleEnd = std::string(" {");
  static auto hostNeedleStart = std::string("  client-hostname \"");
  static auto hostNeedleEnd = std::string("\";");
  if ((queryType==dns::RDATA_A || queryType==dns::RDATA_ANY) && (queryClass==dns::QCLASS_IN || queryClass==dns::QCLASS_ASTERISK)) {
    bcl::FileUtil::ReadByLines(Settings::getDhcpLeases(),[&lastIp,&host,&matchedIp](const std::string& line) {
      if (bcl::StringUtil::startsWith(line,leaseNeedleStart) && bcl::StringUtil::endsWith(line,leaseNeedleEnd)) {
        lastIp=line.substr(leaseNeedleStart.length(),line.length()-leaseNeedleStart.length()-leaseNeedleEnd.length());
      } else if (bcl::StringUtil::startsWith(line,hostNeedleStart) && bcl::StringUtil::endsWith(line,hostNeedleEnd)) {
        auto thisHost=line.substr(hostNeedleStart.length(),line.length()-hostNeedleStart.length()-hostNeedleEnd.length());
        if (thisHost==host) {
          matchedIp=lastIp;
        }
      }
    });
  }

  message.setQr(1);
  if (matchedIp.empty()) {
    // A and AAAA will be requested at once. Do not send NXDOMAIN on AAAA requests, so client won't override the A response
    if (queryType==dns::RDATA_A) {
      message.setRCode(3);
    }
  } else {
    auto *rr = new dns::ResourceRecord();
    rr->setType(dns::RDATA_A);
    rr->setClass(dns::CLASS_IN);
    rr->setTtl(60);
    auto *rdata = new dns::RDataA();
    rdata->setAddress(matchedIp.c_str());
    rr->setRData(rdata);
    message.addAnswer(rr);
  }

  return true;
}

}