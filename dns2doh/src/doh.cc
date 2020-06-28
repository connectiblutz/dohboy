#include "doh.h"

namespace dns2doh {

void DoH::Lookup(dns::Message& message) {
  message.setRCode(3);
}

}