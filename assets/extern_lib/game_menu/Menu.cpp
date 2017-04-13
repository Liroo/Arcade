#include <cmath>
#include "Menu.h"
#include "exceptions/ArcadeException.hpp"
#include "components/Score.h"

Arcade::Menu::Menu():
  _play("menu:play"),
  _pseudo("menu:pseudo"),
  _game("menu:game"),
  _graphic("menu:graphic"),
  _exit("menu:exit"),
  _vader("menu:vader", std::string(DIR_RESSOURCES) + "animated_vader/"),
  _personalScore("menu:personalScore"),
  _scores("menu:scores"),
  _availableLibPopUp("menu:libPopUp") {
  // pointer handler to handleEvent
  _key[Arcade::KeyType::KEY_DOWN] = [this]() -> GameEvent {
    return _keyDown();
  };
  _key[Arcade::KeyType::KEY_UP] = [this]() -> GameEvent {
    return _keyUp();
  };
  _key[Arcade::KeyType::KEY_LEFT] = [this]() -> GameEvent {
    return _keyLeft();
  };
  _key[Arcade::KeyType::KEY_RIGHT] = [this]() -> GameEvent {
    return _keyRight();
  };
  _key[Arcade::KeyType::KEY_ENTER] = [this]() -> GameEvent {
    return _keyEnter();
  };

  _background.id = "menu:background";
  _cursorPosition = 0;
}

Arcade::Menu::~Menu() {
  _timer.stop();
}

bool  Arcade::Menu::_updateDirLib() {
  // update directory listing
  if (!DirectoryReader::updateDirectoryContent(DIR_LIB, _availableLib)
      || !DirectoryReader::updateDirectoryContent(DIR_GAMES, _availableGame)) {
    return false;
  }
  return true;
}

Arcade::GameEvent Arcade::Menu::start(const std::string& libName, const std::string& pseudo) {
  (void)libName;
  _inputPseudo = pseudo;
  // init styles of objects
  _background.position = { 0, 0 };
  _background.size = { 1280, 720 };
  _background.backgroundColor = 0xE3EDF0;
  _background.elevation = 1;

  _backgroundPopUp.id = "menu:backgroundPopUp";
  _backgroundPopUp.backgroundColor = 0xD8D8D8;

  _arrowPopUp.id = "menu:arrowPopUp:1";
  _arrowPopUp.imageName = std::string(DIR_RESSOURCES) + "simple_square.png";
  _arrowPopUp.size = { 40, 40 };
  _arrowPopUp.imageRotation = (M_PI/4); // 45 degrees

  _availableLibPopUp.setBackgroundColor(0x92937D);
  _availableLibPopUp.setShadowColor(0x565748);
  _availableLibPopUp.setFontSize(18);

  _personalScore.setSize({ 271, 101 });
  _personalScore.setPosition({ 508, 84 });
  _personalScore.setBackgroundColor(0xACA8A8);
  _personalScore.setShadowColor(0x726B6B);
  _personalScore.setFontSize(20);

  _scores.setSize({ 254, 225 });
  _scores.setPosition({ 516, 192 });
  _scores.setBackgroundColor(0x92937D);
  _scores.setShadowColor(0x565748);
  _scores.setFontSize(18);

  _play.setSize({ 330, 60 });
  _play.setPosition({ 850, 50 });
  _play.setBackgroundColor(0xFF7777);
  _play.setFocusColor(0xF9A6A6);
  _play.setShadowColor(0xC75959);
  _play.setFontSize(40);
  _play.setText("PLAY");

  _pseudo.setSize({ 215, 60 });
  _pseudo.setPosition({ 907, 208 });
  _pseudo.setBackgroundColor(0x7A7878);
  _pseudo.setFocusColor(0xAAA3A3);
  _pseudo.setShadowColor(0x454141);
  _pseudo.setFontSize(30);

  _game.setSize({ 420, 60 });
  _game.setPosition({ 805, 360 });
  _game.setBackgroundColor(0x8977FF);
  _game.setFocusColor(0x9C8EF8);
  _game.setShadowColor(0x6759C7);

  _graphic.setSize({ 420, 60 });
  _graphic.setPosition({ 805, 450 });
  _graphic.setBackgroundColor(0x8977FF);
  _graphic.setFocusColor(0x9C8EF8);
  _graphic.setShadowColor(0x6759C7);

  _exit.setSize({ 270, 50 });
  _exit.setPosition({ 880, 590 });
  _exit.setBackgroundColor(0x77C4FF);
  _exit.setFocusColor(0x95CDF8);
  _exit.setShadowColor(0x59A5C7);
  _exit.setFontSize(30);
  _exit.setText("EXIT");

  _vader.setSize({ 733, 550 });
  _vader.setPosition({ 37, 85 });

  if (!_vader.init() || !_updateDirLib() ||
    _availableLib.empty() || _availableGame.empty()) {
      throw Arcade::Exception::ArcadeException(ERR_MENUNOTAVAILABLEBECAUSELIB); // ERROR
  }
  for (auto it = _availableGame.begin(); it < _availableGame.end(); it++) {
    *it = (*it).substr(6);
  }
  for (auto it = _availableLib.begin(); it < _availableLib.end(); it++) {
    *it = (*it).substr(4);
  }
  _game.setTextList(_availableGame);
  _graphic.setTextList(_availableLib);

  appendListToList(_objects, _play.render());
  appendListToList(_objects, _pseudo.render());
  appendListToList(_objects, _game.render());
  appendListToList(_objects, _graphic.render());
  appendListToList(_objects, _exit.render());
  appendObjectToList(_objects, _vader.render());
  appendObjectToList(_objects, _background);
  _timer.start();
  return {
    Arcade::EventType::DISPLAY,
    _objects
  };
}

Arcade::GameEvent Arcade::Menu::tick() {
  _render();
  _renderPopUp();
  return {
    Arcade::EventType::DISPLAY,
    _objects
  };
}

Arcade::GameEvent Arcade::Menu::dump() const {
  return {
    Arcade::EventType::DISPLAY,
    _objects
  };
}

Arcade::GameEvent Arcade::Menu::handleEvent(const Event& event) {
  try {
    if (event.key == Arcade::KeyType::KEY_UNKNOWN) {
      return _handleInput(event);
    } else {
      return _key.at(event.key)();
    }
  } catch (std::out_of_range) {
    return {
      Arcade::EventType::DISPLAY,
      _objects
    };
  }
}

void Arcade::Menu::_render() {
  // basic rendering
  appendListToList(_objects, _play.render());
  _pseudo.setText(!_inputPseudo.empty() ? _inputPseudo : "Enter Name");
  appendListToList(_objects, _pseudo.render());
  appendListToList(_objects, _game.render());
  appendListToList(_objects, _graphic.render());
  appendListToList(_objects, _exit.render());
  switch (_cursorPosition) {
    case 0:
      appendListToList(_objects, _play.renderFocus());
      break;
    case 1:
      appendListToList(_objects, _game.renderFocus());
      break;
    case 2:
      appendListToList(_objects, _graphic.renderFocus());
      break;
    case 3:
      appendListToList(_objects, _exit.renderFocus());
      break;
  };
  appendObjectToList(_objects, _vader.renderNext(_timer.getTick()));
}

void Arcade::Menu::_renderPopUp() {
  // popup rendering
  if (_cursorPosition == 1) {
    _availableLibPopUp.setPosition({ 516, 424 });
    _availableLibPopUp.setSize({ 254, 20 + ((_availableGame.size()) * 60) });

    std::string buffer;
    for (auto it = _availableGame.begin(); it < _availableGame.end(); it++) {
      if (*it == _game.getTextLayout()) {
        buffer += "> ";
      } else {
        buffer += "   ";
      }
      buffer += *it + "\n";
    }
    _availableLibPopUp.setText(buffer);

    if (!_inputPseudo.empty()) {
      std::string game = _game.getTextLayout();
      int personalScore = Arcade::Score::getPersonalScoreForGame(game, _inputPseudo);
      _personalScore.setText(_inputPseudo + " : " + std::string(std::to_string(personalScore)));
    }

    _backgroundPopUp.size = { 297, 380 + _availableLibPopUp.getSize().second };
    _backgroundPopUp.position = { 495, 64 };
    _backgroundPopUp.elevation = 7;
    _arrowPopUp.position = { 757, 367 };
    _arrowPopUp.elevation = 8;
    appendListToList(_objects, _availableLibPopUp.render());
    appendListToList(_objects, _personalScore.render());
    appendListToList(_objects, _scores.renderPressed());
  } else if (_cursorPosition == 2) {
    _backgroundPopUp.size = { 297, 20 + ((_availableLib.size()) * 60) };
    _backgroundPopUp.position = { 495, 475 - _backgroundPopUp.size.second/2 };
    _backgroundPopUp.elevation = 7;
    _availableLibPopUp.setSize({ 254, _backgroundPopUp.size.second - 40 });
    _availableLibPopUp.setPosition({ 516, _backgroundPopUp.position.second + 20 });
    _arrowPopUp.position = { 757, 457 };
    _arrowPopUp.elevation = 8;

    std::string buffer;
    for (auto it = _availableLib.begin(); it < _availableLib.end(); it++) {
      if (*it == _graphic.getTextLayout()) {
        buffer += "> ";
      } else {
        buffer += "   ";
      }
      buffer += *it + "\n";
    }
    _availableLibPopUp.setText(buffer);

    appendListToList(_objects, _availableLibPopUp.render());
    appendListToList(_objects, _personalScore.renderHidden());
    appendListToList(_objects, _scores.renderHidden());
  } else {
    _backgroundPopUp.elevation = -1;
    _arrowPopUp.elevation = -1;
    appendListToList(_objects, _availableLibPopUp.renderHidden());
    appendListToList(_objects, _personalScore.renderHidden());
    appendListToList(_objects, _scores.renderHidden());
  }
  appendObjectToList(_objects, _backgroundPopUp);
  appendObjectToList(_objects, _arrowPopUp);
}

Arcade::GameEvent Arcade::Menu::_handleInput(const Event& e) {
  if ((e.data > 0 && e.data < 27) || e.data == 63) {
    if (e.data != 63 && (int)_inputPseudo.size() < 6) {
      _inputPseudo += (e.data + 96);
    } else if (e.data == 63) {
      if (!_inputPseudo.empty())
      _inputPseudo.pop_back();
    }
  }
  return {
    Arcade::EventType::DISPLAY,
    _objects
  };
}


void Arcade::Menu::reset() {
  if (!_updateDirLib() || _availableLib.empty() || _availableGame.empty()) {
    throw Arcade::Exception::ArcadeException(ERR_MENUNOTAVAILABLEBECAUSELIB); // ERROR
  }
  for (auto it = _availableGame.begin(); it < _availableGame.end(); it++) {
    *it = (*it).substr(6);
  }
  for (auto it = _availableLib.begin(); it < _availableLib.end(); it++) {
    *it = (*it).substr(4);
  }
  _timer.reset();
  _game.setTextList(_availableGame);
  _graphic.setTextList(_availableLib);
  _cursorPosition = 0;
}

std::map<std::string, std::string>  Arcade::Menu::dumpMemory() const {
  return {
    { "gameSelected", std::string(DIR_GAMES) + _game.getTextLayout() },
    { "libSelected", std::string(DIR_LIB) + _graphic.getTextLayout() },
    { "pseudo", _inputPseudo }
  };
}

Arcade::GameEvent Arcade::Menu::_keyDown() {
  appendListToList(_objects, _play.render());
  appendListToList(_objects, _game.render());
  appendListToList(_objects, _graphic.render());
  appendListToList(_objects, _exit.render());
  _cursorPosition++;
  if (_cursorPosition >= 4) {
    _cursorPosition = 0;
  }
  switch (_cursorPosition) {
    case 0:
      appendListToList(_objects, _play.renderFocus());
      break;
    case 1:
      appendListToList(_objects, _game.renderFocus());
      break;
    case 2:
      appendListToList(_objects, _graphic.renderFocus());
      break;
    case 3:
      appendListToList(_objects, _exit.renderFocus());
      break;
  };
  return {
    Arcade::EventType::DISPLAY,
    _objects
  };
}

Arcade::GameEvent Arcade::Menu::_keyUp() {
  appendListToList(_objects, _play.render());
  appendListToList(_objects, _game.render());
  appendListToList(_objects, _graphic.render());
  appendListToList(_objects, _exit.render());
  _cursorPosition--;
  if (_cursorPosition < 0) {
    _cursorPosition = 3;
  }
  switch (_cursorPosition) {
    case 0:
      appendListToList(_objects, _play.renderFocus());
      break;
    case 1:
      appendListToList(_objects, _game.renderFocus());
      break;
    case 2:
      appendListToList(_objects, _graphic.renderFocus());
      break;
    case 3:
      appendListToList(_objects, _exit.renderFocus());
      break;
  };
  return {
    Arcade::EventType::DISPLAY,
    _objects
  };
}

Arcade::GameEvent Arcade::Menu::_keyLeft() {
  if (_cursorPosition == 1) {
    appendListToList(_objects, _game.renderPressedLeft());
  } else if (_cursorPosition == 2) {
    appendListToList(_objects, _graphic.renderPressedLeft());
  }
  return {
    Arcade::EventType::DISPLAY,
    _objects
  };
}

Arcade::GameEvent Arcade::Menu::_keyRight() {
  if (_cursorPosition == 1) {
    appendListToList(_objects, _game.renderPressedRight());
  } else if (_cursorPosition == 2) {
    appendListToList(_objects, _graphic.renderPressedRight());
  }
  return {
    Arcade::EventType::DISPLAY,
    _objects
  };
}

Arcade::GameEvent Arcade::Menu::_keyEnter() {
  if (_cursorPosition == 0) {
    return {
      Arcade::EventType::PLAY,
      {}
    };
  } else if (_cursorPosition == 3) {
    return {
      Arcade::EventType::EXIT,
      {}
    };
  }
  return {
    Arcade::EventType::DISPLAY,
    _objects
  };
}
