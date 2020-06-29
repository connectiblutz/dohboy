#include <bcl/consolehandler.h>
#include "worker.h"
#include <string>
#include <unistd.h>
#include <cstring>

int main(int argc, char** argv) {  
  if (argc<2 || strcmp(argv[1],"--debug")) {
    if (fork()!=0) return 0;
  }
  auto runnerThread = std::make_shared<dns2doh::Worker>();
  auto mt = std::dynamic_pointer_cast<bcl::MessageThreadPool>(runnerThread);
  auto ch = bcl::Singleton::Weak::create<bcl::ConsoleHandler>(mt);
  runnerThread->join();
  return 0;
}