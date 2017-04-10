#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
// custom includes
#include "unistd.h"
#include "Core.h"
#include "exceptions/ArcadeException.hpp"

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
  if (!DirectoryReader::updateDirectoryContent(DIR_LIB, _availableLib)
      || !DirectoryReader::updateDirectoryContent(DIR_GAMES, _availableGame)) {
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
  _dlGame.reset(new LibLoader<AGames>(DFL_ENTRY_NAME));
  return true;
}

int Core::run() {
  // load first Libs
  if (_loadGame() == -1 || _loadLib() == -1) {
    return 1;
  }
  while (_isRunning) {
    if (_game) {
      try {
        _runGame();
      } catch (const Arcade::Exception::ArcadeException &e) {
        std::cout << e.what() << std::endl;
        return 0;
      }
    }
    if (_graphic && _runLib() == -1) {
      return 1;
    }
  }
  return 0;
}

// Dynamic Library handling

void Core::_iterateIndex(const DirectoryReader::DirectoryContent& libContent,
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
  _game.reset(_dlGame->openLib(_isMenu ? MENU_PATH :
      _availableGame.at(_availableGameIndex)));
  if (!_game) {
    return -1;
  }
  return 0;
}

int Core::_loadLib() {
  if (_dlGraphic->isOpen()) {
    _dlGraphic->closeLib();
  }
  if (_graphic) {
    _graphic->close();
  }
  _graphic.reset(_dlGraphic->openLib(_availableLib.at(_availableLibIndex)));
  if (!_graphic) {
    return -1;
  }
  return 0;
}

void Core::_runGame() {
  if (_game) {
    _game->start([this](const Event& event) -> void {
      _handleEvent(event);
    });
  }
}

int Core::_runLib() {
  try {
    _graphic->init([this](const Event& event) -> void {
      _handleEvent(event);
    });
    _graphic->run();
  } catch (const Arcade::Exception::ArcadeException &e) {
    std::cerr << e.what() << std::endl;
    return -1;
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
    case Arcade::EventType::PLAY:
      _playMenu();
      break;
    case Arcade::EventType::EXIT:
      _exit();
      break;
    case Arcade::EventType::UNKNOWN:
      std::clog << UNK_EVNT << std::endl;
      break;
  }
}

void Core::_handleTick() {
  if (!_game || !_graphic) {
    return;
  }
  // tick the game
  ObjectList objects = _game->tick();
  _graphic->update(objects);
}

// not so heavy fn
void Core::_handleKey(const Event& event) {
  if (!_game || !_graphic) {
    return;
  }
  if (event.key <= KEY_ESC) {
    // update directory listing
    if (!DirectoryReader::updateDirectoryContent(DIR_LIB, _availableLib)
        || !DirectoryReader::updateDirectoryContent(DIR_GAMES, _availableGame)) {
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
  if (event.key != Arcade::KeyType::KEY_ENTER && _graphic) {
    _graphic->update(objects);
  }
}

void Core::_handleResize() {
  if (!_game || !_graphic) {
    return;
  }
  // dumping of screen due to important changement on graphics
  ObjectList objects = _game->dump();
  _graphic->update(objects);
}

// Core event
void Core::_changeLib(int offset) {
  int indexCleat = _availableLib.size();
  _iterateIndex(_availableLib, _availableLibIndex, offset);
  while (indexCleat < 0) {
    if (_loadLib() == 0) {
      if (_graphic) {
        _graphic->close();
        for (int i = 0; i < 5; i++) {
          usleep(500);
          if (_graphic->canBeDeleted()) {
            _graphic.reset(nullptr);
          }
        }
        _graphic.reset(nullptr);
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
        _game.reset(nullptr);
        break;
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
    try {
      _game->reset();
    } catch (const Arcade::Exception::ArcadeException &e) {
      std::cout << e.what() << std::endl;
      _exit();
    }
  }
}

void Core::_startMenu() {
  _isMenu = true;
  if (_loadGame() == -1) {
    std::cerr << ERR_MENUNOTAVAILABLE << std::endl;
    _exit();
  }
}

void Core::_playMenu() {
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
    for (int i = 0; i < 5; i++) {
      usleep(500);
      if (_graphic->canBeDeleted()) {
        break;
      }
    }
  }
}
