#pragma once

#include <message.h>

namespace dns2doh {

class DoH {
  public:
    static void Lookup(dns::Message& message); 
};

}