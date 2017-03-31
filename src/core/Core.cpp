#include <string>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <unordered_set>

// C includes
#include "dirent.h"
#include "sys/stat.h"

// custom includes
#include "Core.hpp"

using namespace Arcade;

Core::Core():
_timer(std::chrono::milliseconds(TICK_MS)) {}

Core::~Core() {}

bool Core::init(const std::string& libName) {
  // update directory listing
  if (!updateDirectoryContent(DIR_LIB, _availableLib)
      || !updateDirectoryContent(DIR_GAMES, _availableGame)) {
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

/*
  This custom algo is O(n) + O(k) speed (didn't found a fastest way to do what I want)
    ~ anyway O(n) + O(k) is actually pretty fast

  where:
    - n is the numbers of file already stored
    - k is the numbers of file in directory
*/
bool Core::updateDirectoryContent(const char* directoryName,
    std::vector<std::string>& libData) {
  DIR *dir;

  if ((dir = opendir(directoryName)) == NULL) {
    // ERROR
    std::cerr << ERR_READDIR << directoryName << std::endl;
    return false;
  }

  struct dirent *ent;
  struct stat fileStat;
  std::unordered_set<std::string> regularFiles;

  // store all the files within directory
  while ((ent = readdir(dir)) != NULL) {
    if (ent->d_name[0] == '.') {
      continue;
    }
    std::string relativeFilePath = std::string(directoryName) + ent->d_name;
    stat(relativeFilePath.c_str(), &fileStat);
    if (!S_ISREG(fileStat.st_mode)) {
      continue;
    }
    regularFiles.insert(relativeFilePath);
    if (!std::binary_search(libData.begin(), libData.end(), relativeFilePath)) {
      libData.push_back(relativeFilePath);
    }
  }
  closedir(dir);
  // custom for_each getting iterator as value
  // useful to clear memory of old library deleted during runtime
  for (auto i = libData.begin(); i < libData.begin(); i++) {
    if (!std::binary_search(regularFiles.begin(), regularFiles.end(), *i)) {
      libData.erase(i);
    }
  }
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

//    std::cout << "running" << std::endl;
  }
  std::cout << "Over" << std::endl;
  return 0;
}
