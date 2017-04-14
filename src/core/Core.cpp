#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
// custom includes
#include "unistd.h"
#include "Core.h"
#include "exceptions/ArcadeException.hpp"

using namespace Arcade;

Core::Core():
  _dlGame(DFL_ENTRY_NAME),
  _dlGraphic(DFL_ENTRY_NAME) {}
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
  _currentPseudo = "";
  _currentGame = "";
  _gameChanging = false;
  _gameWorking = false;
  _nextGame = NULL;
  _nextGraphic = NULL;
  return true;
}

int Core::run() {
  // load first Libs
  if (_loadGame() == -1 || _loadLib() == -1) {
    return 1;
  }
  try {
    _runGame();
  } catch (const Arcade::Exception::ArcadeException &e) {
    std::cout << e.what() << std::endl;
    return 0;
  }
  while (_isRunning) {
    if (_runLib() == -1) {
      return 1;
    }
    _deleteLib();
    if (_isRunning && _loadLib() == -1) {
      _exit();
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

int Core::_loadLib() {
  Arcade::IGraphic* graphic = _dlGraphic.openLib(_availableLib.at(_availableLibIndex));
  if (!graphic) {
    return -1;
  }
  _nextGraphic = graphic;
  return 0;
}

int Core::_runLib() {
  _graphic.reset(_nextGraphic);
  try {
    _graphic->init([=](const Event& event) -> void {
      _handleEvent(event);
    });
    _graphic->run();
  } catch (const Arcade::Exception::ArcadeException &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  return 0;
}

int Core::_deleteLib() {
  if (!_graphic) {
    return 0;
  }
  int i = 0;
  for (; i < 10; i++) {
    usleep(200000);
    if (_graphic->isDeletable()) {
      break;
    }
  }
  if (i == 10) {
    _forceExit();
  }
  _graphic.reset(nullptr);
  return 0;
}

int Core::_loadGame() {
  Arcade::AGames* game = _dlGame.openLib(_isMenu ? MENU_PATH :
    _availableGame.at(_availableGameIndex));
  if (!game) {
    return -1;
  }
  _nextGame = game;
  return 0;
}

void Core::_runGame() {
  _game.reset(_nextGame);
  if (_game) {
    _gameWorking = true;
    _game->start(_currentGame, _currentPseudo);
    _gameWorking = false;
  }
}

int Core::_deleteGame() {
  if (!_game) {
    return 0;
  }
  int i = 0;
  for (; i < 1000000; i++) {
    if (!_gameWorking) {
      break;
    }
  }
  if (i == 1000000) {
    _forceExit();
  }
  _game.reset(nullptr);
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
    default:
      break;
  }
}

void Core::_handleTick() {
  if (!_game || !_graphic || _gameChanging) {
    return;
  }
  // tick the game
  _gameWorking = true;
  GameEvent gEvent = _game->tick();
  _gameWorking = false;
  if (_graphic) {
    _graphic->update(gEvent.objects);
  }
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
  if (_gameChanging) {
    return;
  }
  _gameWorking = true;
  GameEvent gEvent = _game->handleEvent(event);
  _gameWorking = false;
  switch (gEvent.type) {
    case Arcade::EventType::DISPLAY:
      if (_graphic) {
        _graphic->update(gEvent.objects);
      }
      break;
    case Arcade::EventType::PLAY:
      _playMenu();
      break;
    case Arcade::EventType::EXIT:
      _exit();
      break;
    default:
      break;
  }
}

void Core::_handleResize() {
  if (!_game || _gameChanging) {
    return;
  }
  // dumping of screen due to important changement on graphics
  _gameWorking = true;
  GameEvent gEvent = _game->dump();
  _gameWorking = false;
  if (_graphic) {
    _graphic->update(gEvent.objects);
  }
}

// Core event
void Core::_changeLib(int offset) {
  if (offset == 0) {
    return;
  }
  int oldIndex = _availableLibIndex;
  _iterateIndex(_availableLib, _availableLibIndex, offset);
  if (oldIndex == _availableLibIndex) {
    return ;
  }
  if (_graphic) {
    _graphic->close();
  }
}

void Core::_changeGame(int offset) {
  if (offset == 0 || _isMenu) {
    return;
  }

  _gameChanging = true;
  int oldIndex = _availableGameIndex;
  _iterateIndex(_availableGame, _availableGameIndex, offset);

  if (oldIndex == _availableGameIndex) {
    return;
  }
  _deleteGame();
  if (_loadGame() == -1) {
    _exit();
    return;
  }
  _runGame();
  _gameChanging = false;
}

void Core::_restartGame() {
  if (_game && !_gameChanging) {
    try {
      _gameWorking = true;
      _game->reset();
      _gameWorking = false;
    } catch (const Arcade::Exception::ArcadeException &e) {
      std::cout << e.what() << std::endl;
      _exit();
    }
  }
}

void Core::_startMenu() {
  if (_isMenu) {
    return;
  }

  _gameChanging = true;
  _isMenu = true;
  _deleteGame();
  _availableGameIndex = -1;
  if (_loadGame() == -1) {
    _exit();
    return;
  }
  _runGame();
  _gameChanging = false;
}

void Core::_playMenu() {
  if (_gameChanging) {
    return ;
  }
  _gameWorking = true;
  std::map<std::string, std::string> config = _game->dumpMemory();
  _gameWorking = false;

  std::string libSelected = config.at("libSelected");
  auto libIt = std::find(_availableLib.begin(), _availableLib.end(), libSelected);
  if (libIt == _availableLib.end()) {
    std::clog << WRN_LIBNOTAVAILABLE << std::endl;
    _restartGame();
    return;
  }
  _changeLib(std::distance(_availableLib.begin() + _availableLibIndex,
    libIt));

  std::string gameSelected = config.at("gameSelected");
  auto gameIt = std::find(_availableGame.begin(), _availableGame.end(), gameSelected);
  if (gameIt == _availableGame.end()) {
    std::clog << WRN_GAMENOTAVAILABLE << std::endl;
    _restartGame();
    return;
  }
  _isMenu = false;
  _currentGame = gameSelected.substr(6);
  _currentPseudo = config.at("pseudo");
  _changeGame(std::distance(_availableGame.begin() + _availableGameIndex,
    gameIt));
}

void Core::_exit() {
  _isRunning = false;
  if (_graphic) {
    _graphic->close();
  }
}

void Core::_forceExit() {
  std::cerr << FATAL_ERROR << std::endl;
  exit(1);
}
