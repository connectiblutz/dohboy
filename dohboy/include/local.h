#pragma once

#include <message.h>

namespace dohboy {

class Local {
  public:
    static bool Lookup(dns::Message& message); 
};

}