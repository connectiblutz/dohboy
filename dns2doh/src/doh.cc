#include "doh.h"
#include <curl/curl.h>
#include <bcl/logutil.h>
#include <cstring>
#include <sstream>
#include "settings.h"

namespace dohboy {

struct write_buffer {
  size_t len;
  char* buf;
};
size_t write_data(void *buffer, size_t size, size_t nmemb, write_buffer* userp)
{
  memcpy(userp->buf+userp->len,buffer,size*nmemb);
  userp->len+=size * nmemb;
  return size * nmemb;
}

void DoH::Lookup(dns::Message& message) {
  message.setRCode(3);

  thread_local CURL *curl = nullptr;

  CURLcode res;
 
  if (!curl) {
    // one time setup per thread. these will leak on shutdown, but we're exiting everything then anyway
    curl = curl_easy_init();
    std::ostringstream resolvess;
    resolvess << Settings::getDomain()<< ":443:" << Settings::getDomainIP().toString();
    std::string resolve = resolvess.str();

    std::ostringstream surl;
    surl << "https://" << Settings::getDomain() << "/dns-query";
    std::string url = surl.str();
    auto hosts = curl_slist_append(nullptr, resolve.c_str());
    curl_easy_setopt(curl, CURLOPT_RESOLVE, hosts);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    auto headers = curl_slist_append(nullptr, "Content-Type: application/dns-message");
    headers = curl_slist_append(headers, "Accept: application/dns-message");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  }

  if(curl) {
    auto response = std::make_unique<char[]>(4096);
    write_buffer response_buffer;
    response_buffer.len=0;
    response_buffer.buf=response.get();
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_buffer);
 
    auto data = std::make_unique<char[]>(4096);
    dns::uint size;
    message.encode(data.get(),4096,size);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.get());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, size);
 
    res = curl_easy_perform(curl);
 
    if(res != CURLE_OK) {
      bcl::LogUtil::Debug()<<"curl_easy_perform() failed: " << curl_easy_strerror(res);
    } else {
      long response_code;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
      bcl::LogUtil::Debug()<<"doh success "<<response_code;
      if (response_code==200) {
        message.decode(response_buffer.buf,response_buffer.len);
        bcl::LogUtil::Debug()<<"doh response size "<<response_buffer.len<<" with "<<message.getAnCount()<<" answers";
        message.setRCode(0);
      }
    }
  }
}

}
