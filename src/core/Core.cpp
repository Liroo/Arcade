#include <string>
#include <iostream>
#include <chrono>
#include <algorithm>

// custom includes
#include "Core.hpp"

using namespace Arcade;

Core::Core():
_timer(std::chrono::milliseconds(TICK_MS)) {}

Core::~Core() {}

bool Core::init(const std::string& libName) {
  // update directory listing
  if (!DirectoryLib::updateDirectoryContent(DIR_LIB, _availableLib)
      || !DirectoryLib::updateDirectoryContent(DIR_GAMES, _availableGame)) {
    return false;
  }
  // find correct libName
  auto it = std::find_if(_availableLib.begin(), _availableLib.end(), [&libName](std::string item) -> bool {
    // ensure by both ways the lib is not present in fs
    if (item == libName || std::string("./") + item == libName) {
      return true;
    }
    return false;
  });
  if (it == _availableLib.end()) {
    // ERROR
    std::cerr << ERR_STARTEDLIB << libName << std::endl;
    return false;
  }
  // setting index
  _availableLibIndex = std::distance(_availableLib.begin(), it);
  return true;
}

int Core::run() {
  // main loop
  while (_isRunning) {
    // if timer was stoped by another things
    // don't do anything and leave program
    if (!(_isRunning = _timer.waitFor())) {
      break;
    }

    // do here something like
    /*
      std::vector<Arcade::Object *> toUpdate = _game.tick();
      _graphicLib.update(toUpdate);
    */

    //  std::cout << "running" << std::endl;
  }
  std::cout << "Over" << std::endl;
  return 0;
}
