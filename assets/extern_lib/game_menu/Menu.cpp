#include "Menu.h"
#include "exceptions/ArcadeException.hpp"

Arcade::Menu::Menu():
  _play("play"),
  _game("game"),
  _graphic("graphic"),
  _exit("exit"),
  _vader("vader", std::string(DIR_RESSOURCES) + "animated_vader/") {
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

  _background.id = "background";
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

Arcade::GameEvent Arcade::Menu::start() {
  // init styles of objects
  _background.position = { 0, 0 };
  _background.size = { 1280, 720 };
  _background.backgroundColor = 0xE3EDF0;
  _background.elevation = 1;

  _play.setSize({ 430, 80 });
  _play.setPosition({ 180, 85 });
  _play.setBackgroundColor(0xFF7777);
  _play.setFocusColor(0xF9A6A6);
  _play.setShadowColor(0xC75959);
  _play.setFontSize(42);
  _play.setText("PLAY");

  _exit.setSize({ 430, 80 });
  _exit.setPosition({ 180, 555 });
  _exit.setBackgroundColor(0x77C4FF);
  _exit.setFocusColor(0x95CDF8);
  _exit.setShadowColor(0x59A5C7);
  _exit.setFontSize(42);
  _exit.setText("EXIT");

  _game.setSize({ 600, 80 });
  _game.setPosition({ 95, 250 });
  _game.setBackgroundColor(0x8977FF);
  _game.setFocusColor(0x9C8EF8);
  _game.setShadowColor(0x6759C7);

  _graphic.setSize({ 600, 80 });
  _graphic.setPosition({ 95, 390 });
  _graphic.setBackgroundColor(0x8977FF);
  _graphic.setFocusColor(0x9C8EF8);
  _graphic.setShadowColor(0x6759C7);

  _vader.setSize({ 733, 550 });
  _vader.setPosition({ 609, 85 });

  if (!_vader.init() || !_updateDirLib() ||
    _availableLib.empty() || _availableGame.empty()) {
      throw Arcade::Exception::ArcadeException(ERR_MENUNOTAVAILABLEBECAUSELIB); // ERROR
  }
  _game.setTextList(_availableGame);
  _graphic.setTextList(_availableLib);

  appendListToList(_objects, _play.render());
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
  appendListToList(_objects, _play.render());
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
    return _key.at(event.key)();
  } catch (std::out_of_range) {
    return {};
  }
}

void Arcade::Menu::reset() {
  if (!_updateDirLib() || _availableLib.empty() || _availableGame.empty()) {
    throw Arcade::Exception::ArcadeException(ERR_MENUNOTAVAILABLEBECAUSELIB); // ERROR
  }
  _timer.reset();
  _game.setTextList(_availableGame);
  _graphic.setTextList(_availableLib);
  _cursorPosition = 0;
}

std::map<std::string, std::string>  Arcade::Menu::dumpMemory() const {
  return {
    { "gameSelected", _game.getTextLayout() },
    { "libSelected", _graphic.getTextLayout() }
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
