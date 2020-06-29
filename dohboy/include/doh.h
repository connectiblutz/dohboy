#pragma once

#include <message.h>

namespace dohboy {

class DoH {
  public:
    static void Lookup(dns::Message& message); 
};

}