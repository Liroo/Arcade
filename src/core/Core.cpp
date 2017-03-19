#include <string>
#include <iostream>
#include <chrono>

#include "Core.hpp"

using namespace Arcade;

Core::Core(const std::string &libName):
_timer(std::chrono::milliseconds(10)) {
  _libName = libName;
}

Core::~Core() {}

int Core::run() {
  // main loop
  while (_isRunning) {
    // if timer was stoped by another things
    // don't do anything and leave program
    if (!(_isRunning = _timer.waitFor())) {
      break;
    }
    std::cout << "running" << std::endl;
  }
  std::cout << "Over" << std::endl;
  return 0;
}
