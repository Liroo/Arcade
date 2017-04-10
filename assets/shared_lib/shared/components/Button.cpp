#include "Button.h"

using namespace Arcade;

Button::Button(const std::string& id) {
  _background.id = id + ":background";
  _shadow.id = id + ":shadow";
  _background.elevation = 11;
  _shadow.elevation = 10;
  _fontSize = 36;
}

Button::Button(const std::pair<int, int>& size,
  const std::pair<int, int>& position,
  const std::string& id) {
  _background.id = id + ":background";
  _shadow.id = id + ":shadow";
  _background.elevation = 11;
  _shadow.elevation = 10;
  _fontSize = 36;
  _size = size;
  _position = position;
}

Button::~Button() {}

std::pair<int, int> Button::getSize() const {
  return _size;
}

void Button::setSize(const std::pair<int, int>& size) {
  _size = size;
}

std::pair<int, int> Button::getPosition() const {
  return _position;
}

void Button::setPosition(const std::pair<int, int>& position) {
  _position = position;
}

int Button::getBackgroundColor() const {
  return _backgroundColor;
}

void Button::setBackgroundColor(int backgroundColor) {
  _backgroundColor = backgroundColor;
}

int Button::getFocusColor() const {
  return _focusColor;
}

void Button::setFocusColor(int focusColor) {
  _focusColor = focusColor;
}

int Button::getShadowColor() const {
  return _shadowColor;
}

void Button::setShadowColor(int shadowColor) {
  _shadowColor = shadowColor;
}

int Button::getFontSize() const {
  return _fontSize;
}

void Button::setFontSize(int fontSize) {
  _fontSize = fontSize;
}

std::string Button::getText() const {
  return _text;
}

void Button::setText(const std::string& text) {
  _text = text;
}

ObjectList Button::render() {
  std::pair<int, int> bgSize = {
    _size.first,
    (87 * _size.second) / 100 // 87% or width
  };
  // background object
  _background.position = _position;
  _background.size = bgSize;
  _background.backgroundColor = _backgroundColor;
  _background.fontSize = _fontSize;
  _background.text = _text;

  // shadow object
  _shadow.position = {
    _position.first,
    _position.second + bgSize.second + 1
  };
  _shadow.size = {
    _size.first,
    _size.second - bgSize.second
  };
  _shadow.backgroundColor = _shadowColor;
  return ObjectList({
    _background,
    _shadow
  });
}

ObjectList Button::renderFocus() {
  std::pair<int, int> bgSize = {
    _size.first,
    (87 * _size.second) / 100 // 87% or width
  };
  // background object
  _background.position = _position;
  _background.size = bgSize;
  _background.backgroundColor = _focusColor;
  _background.fontSize = _fontSize;
  _background.text = _text;

  // shadow object
  _shadow.position = {
    _position.first,
    _position.second + bgSize.second + 1
  };
  _shadow.size = {
    _size.first,
    _size.second - bgSize.second
  };
  _shadow.backgroundColor = _shadowColor;
  return ObjectList({
    _background,
    _shadow
  });
}

ObjectList Button::renderPressed() {
  std::pair<int, int> bgSize = {
    _size.first,
    (94 * _size.second) / 100 // 87% or width
  };
  // background object
  _background.position = _position;
  _background.size = bgSize;
  _background.backgroundColor = _backgroundColor;
  _background.fontSize = _fontSize;
  _background.text = _text;

  // shadow object
  _shadow.position = {
    _position.first,
    _position.second + bgSize.second
  };
  _shadow.size = {
    _size.first,
    _size.second - bgSize.second
  };
  _shadow.backgroundColor = _shadowColor;
  return ObjectList({
    _background,
    _shadow
  });
}
