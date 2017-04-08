#include <string>
#include <iostream>
#include <algorithm>
#include <functional>

// custom includes
#include "Core.h"

using namespace Arcade;

Core::Core() {}
Core::~Core() {}

bool Core::init(const std::string& libName) {
  if (libName.empty()) {
    std::cout
      << "arcade:" << std::endl
      << "   usage: arcade path/to/first_lib.so" << std::endl
      ;
    return false;
  }
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
  _dlGraphic.reset(new LibLoader<IGraphic>(DFL_ENTRY_NAME));
  _dlGame.reset(new LibLoader<IGames>(DFL_ENTRY_NAME));
  return true;
}

int Core::run() {
  // load first Libs
  if (_loadGame() == -1 || _loadLib() == -1) {
    std::cout << "load" << std::endl;
    return 1;
  }
  while (_isRunning) {
    _runGame();
    if (_runLib() == -1) {
      return 1;
    }
  }
  return 0;
}

// Dynamic Library handling

void Core::_iterateIndex(const DirectoryLib::DirectoryLibContent& libContent,
  int& index, int iteration) {
  // check if iteration is available
  if (iteration > 0 && index + iteration >= (int)libContent.size()) {
    index = 0;
  } else if (iteration < 0 && index + iteration < 0) {
    index = libContent.size() - 1;
  } else {
    index += iteration;
  }
}

int Core::_loadGame() {
  if (_dlGame->isOpen()) {
    _dlGame->closeLib();
  }
  #ifdef DEBUG
    if ((_game = _dlGame->openLib(DEBUG_GAME)) == NULL) {
      return -1;
    }
  #else
    if ((_game = _dlGame->openLib(_isMenu ? MENU_PATH :
        _availableGame.at(_availableGameIndex))) == NULL) {
      return -1;
    }
  #endif
  return 0;
}

int Core::_loadLib() {
  if (_dlGraphic->isOpen()) {
    _dlGraphic->closeLib();
  }
  if ((_graphic = _dlGraphic->openLib(_availableLib.at(_availableLibIndex))) == NULL) {
    return -1;
  }
  return 0;
}

void Core::_runGame() {
  if (_game) {
    _game->start();
  }
}

int Core::_runLib() {
  try {
    if (_graphic) {
      _graphic->close();
    }
    _graphic->init([this](const Event& event) -> void {
      _handleEvent(event);
    });
  } catch (...) {
    std::cerr << ERR_GRAPHICINIT << std::endl;
    return -1;
  }
  // dumping of screen due to changement of game
  ObjectList objects = _game->dump();
  try {
    _graphic->update(objects);
  } catch (...) {
    std::cerr << ERR_GRAPHICUPDATE << std::endl;
  }
  return 0;
}

/*
  Event Handling
  any event send by graphic will not cause a crash except if any library want to be loaded
*/

void Core::_handleEvent(const Event& event) {
  switch (event.type) {
    case Arcade::EventType::RESIZE:
      _handleResize();
      break;
    case Arcade::EventType::KEY_PRESSED:
      _handleKey(event);
      break;
    case Arcade::EventType::TICK:
      _handleTick();
      break;
    case Arcade::EventType::UNKNOWN:
      std::clog << UNK_EVNT << std::endl;
      break;
  }
}

void Core::_handleTick() {
  // tick the game
  ObjectList objects = _game->tick();
  try {
    _graphic->update(objects);
  } catch (...) {
    std::cerr << ERR_GRAPHICUPDATE << std::endl;
  }
}

// not so heavy fn
void Core::_handleKey(const Event& event) {
  if (event.key <= KEY_ESC) {
    // update directory listing
    if (!DirectoryLib::updateDirectoryContent(DIR_LIB, _availableLib)
        || !DirectoryLib::updateDirectoryContent(DIR_GAMES, _availableGame)) {
      return;
    }
    /*
      switch statement version fn pointer:
      http://stackoverflow.com/questions/30061839/is-it-better-to-use-an-array-of-pointers-to-members-or-a-switch

      annnnddd for this big contest and this usabilitym the winner is... fn pointer !
    */
    /*
      this should be many redundant code here but this is necessary
        - first for well comprehension of the code
        - two because there is two (or more) different variable just insane to use
        - three because we don't need optimisation here
    */
    std::map<Arcade::KeyType, std::function<void()>> coreHandling = {
      {Arcade::KeyType::KEY_TWO, [this]() -> void {
        _changeLib(-1);
      }},
      {Arcade::KeyType::KEY_THREE, [this]() -> void {
        _changeLib(+1);
      }},
      {Arcade::KeyType::KEY_FOUR, [this]() -> void {
        _changeGame(-1);
      }},
      {Arcade::KeyType::KEY_FIVE, [this]() -> void {
        _changeGame(+1);
      }},
      {Arcade::KeyType::KEY_EIGHT, [this]() -> void {
        _restartGame();
      }},
      {Arcade::KeyType::KEY_NINE, [this]() -> void {
        _startMenu();
      }},
      {Arcade::KeyType::KEY_ENTER, [this]() -> void {
        _enterKey();
      }},
      {Arcade::KeyType::KEY_ESC, [this]() -> void {
        _exit();
      }},
    };
    try {
      coreHandling.at(event.key)();
    } catch (std::out_of_range) {
      return;
    }
    return;
  }
  ObjectList objects = _game->handleEvent(event);
  try {
    _graphic->update(objects);
  } catch (...) {
    std::cerr << ERR_GRAPHICUPDATE << std::endl;
  }
}

void Core::_handleResize() {
  // dumping of screen due to important changement on graphics
  ObjectList objects = _game->dump();
  try {
    _graphic->update(objects);
  } catch (...) {
    std::cerr << ERR_GRAPHICUPDATE << std::endl;
  }
}

// Core event
void Core::_changeLib(int offset) {
  int indexCleat = _availableLib.size();
  _iterateIndex(_availableLib, _availableLibIndex, offset);
  while (indexCleat < 0) {
    if (_loadLib() == 0) {
      if (_graphic) {
        _graphic->close();
        delete _graphic;
      }
      return;
    }
    _iterateIndex(_availableLib, _availableLibIndex, offset);
    indexCleat -= 1;
  }
  std::cerr << ERR_NOMORELIBAVAILABLE << std::endl;
  _exit();
}

void Core::_changeGame(int offset) {
  if (_isMenu) {
    return;
  }
  int indexCleat = _availableGame.size();
  _iterateIndex(_availableGame, _availableGameIndex, offset);
  while (indexCleat < 0) {
    if (_loadLib() == 0) {
      if (_game) {
        delete _game;
      }
      return;
    }
    _iterateIndex(_availableGame, _availableGameIndex, offset);
    indexCleat -= 1;
  }
  std::cerr << ERR_NOMOREGAMEAVAILABLE << std::endl;
  _exit();
}

void Core::_restartGame() {
  if (_game) {
    _game->reset();
  }
}

void Core::_startMenu() {
  _isMenu = true;
  if (_loadGame() == -1) {
    std::cerr << ERR_MENUNOTAVAILABLE << std::endl;
    _exit();
  }
}

void Core::_enterKey() {
  std::map<std::string, std::string> config = _game->dumpMemory();

  std::string gameSelected = config.at("gameSelected");
  auto gameIt = std::find(_availableGame.begin(), _availableGame.end(), gameSelected);
  if (gameIt == _availableGame.end()) {
    std::clog << WRN_GAMENOTAVAILABLE << std::endl;
    _restartGame();
    return;
  }
  _changeGame(std::distance(_availableGame.begin() + _availableGameIndex,
    gameIt));

  std::string libSelected = config.at("libSelected");
  auto libIt = std::find(_availableLib.begin(), _availableLib.end(), gameSelected);
  if (libIt == _availableLib.end()) {
    std::clog << WRN_LIBNOTAVAILABLE << std::endl;
    _restartGame();
    return;
  }
  _changeLib(std::distance(_availableLib.begin() + _availableLibIndex,
    libIt));

  _isMenu = false;
}

void Core::_exit() {
  _isRunning = false;
  if (_graphic) {
    _graphic->close();
    delete _graphic;
  }
  if (_game) {
    delete _game;
  }
}
