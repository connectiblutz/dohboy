#include <bcl/consolehandler.h>
#include "worker.h"
#include <string>

int main(int argc, char** argv) {
  std::string config;
  if (argc>1) {
    config=argv[1];
  }
  auto runnerThread = std::make_shared<dns2doh::Worker>(config);
  auto mt = std::dynamic_pointer_cast<bcl::MessageThreadPool>(runnerThread);
  auto ch = bcl::Singleton::Weak::create<bcl::ConsoleHandler>(mt);
  runnerThread->join();
  return 0;
}