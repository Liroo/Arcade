#include <iostream>

#include "LibCaca.h"
#include "exceptions/ArcadeException.hpp"

Caca::Caca() {}
Caca::~Caca() {}

void Caca::init(const Arcade::Callback& callback) {
  _callback = callback;
  _isRunning = true;
}

void Caca::run() {
  caca_event_t ev;

  _canvas = caca_create_canvas(128, 36);
  _display = caca_create_display(_canvas);
  if(!_display || !_canvas) {
    throw Arcade::Exception::ArcadeException(ERR_GRAPHICINIT);
  }

  caca_set_display_title(_display, "Arcade");
  caca_refresh_display(_display);
  _isLooping = true;
  while (isRunning()) {
    std::cout << "tick" << std::endl;
    while (caca_get_event(_display, CACA_EVENT_ANY, &ev, 10000)) {
      if (caca_get_event_type(&ev) & CACA_EVENT_KEY_PRESS) {
        _handleEvent(ev);
      }
    }
    _callback({
      Arcade::EventType::TICK,
      Arcade::KeyType::KEY_UNKNOWN,
      0
    });
    caca_refresh_display(_display);
  }
  caca_free_display(_display);
  _isLooping = false;
}

void Caca::close() {
  _isRunning = false;
}

void Caca::update(Arcade::ObjectList objs) {
  if (!_isRunning) {
    return;
  }
  std::sort(objs.begin(), objs.end(), [](Arcade::Object i1, Arcade::Object i2) -> bool {
    return i1.elevation < i2.elevation;
  });
  std::for_each(objs.begin(), objs.end(), [=](Arcade::Object obj) {
    if (obj.elevation > 0) {
      _drawObj(obj);
    }
  });
}

void Caca::_drawObj(Arcade::Object obj) {
  if (obj.backgroundColor >= 0)
    _drawButton(obj);
  if (!obj.rawImage.empty())
    _drawImage(obj);
  if (!obj.text.empty())
    _drawText(obj);
}

void Caca::_drawButton(Arcade::Object obj) {
  int red = (obj.backgroundColor & 0xff0000) >> 16;
  int green = (obj.backgroundColor & 0x00ff00) >> 8;
  int blue = (obj.backgroundColor & 0x0000ff);
  uint16_t BGRColor = red >> 3;
  BGRColor |= (green & 0xFC) << 3;
  BGRColor |= (blue  & 0xF8) << 8;
  caca_set_color_argb(_canvas, 0x0000, BGRColor);
  caca_fill_box(_canvas,
    obj.rawPosition.first, obj.rawPosition.second,
    obj.rawSize.first,
    obj.rawSize.second,
    ' '
  );
}

void Caca::_drawImage(Arcade::Object obj) {
  int red = (obj.backgroundColor & 0xff0000) >> 16;
  int green = (obj.backgroundColor & 0x00ff00) >> 8;
  int blue = (obj.backgroundColor & 0x0000ff);
  uint16_t BGRColor = red >> 3;
  BGRColor |= (green & 0xFC) << 3;
  BGRColor |= (blue  & 0xF8) << 8;
  caca_set_color_argb(_canvas, 0xffff, BGRColor);
  caca_put_str(_canvas,
    obj.rawPosition.first, obj.rawPosition.second,
    obj.rawImage[0].c_str());
}

void Caca::_drawText(Arcade::Object obj) {
  (void)obj;
}

bool Caca::isRunning() const {
  return _isRunning;
}

bool Caca::isDeletable() const {
  return !(_isLooping || _isRunning);
}

bool Caca::isClosed() const {
  return true;
}

void Caca::_handleEvent(caca_event_t e) {
  int key = caca_get_event_key_ch(&e);
  Arcade::KeyType keyPressed = Arcade::KeyType::KEY_UNKNOWN;

  std::cout << key << std::endl;

  switch (key) {
    case '2':
      keyPressed = Arcade::KeyType::KEY_TWO;
      break;
    case '3':
      keyPressed = Arcade::KeyType::KEY_THREE;
      break;
    case '4':
      keyPressed = Arcade::KeyType::KEY_FOUR;
      break;
    case '5':
      keyPressed = Arcade::KeyType::KEY_FIVE;
      break;
    case '8':
      keyPressed = Arcade::KeyType::KEY_EIGHT;
      break;
    case '9':
      keyPressed = Arcade::KeyType::KEY_NINE;
      break;
    case CACA_KEY_ESCAPE:
      keyPressed = Arcade::KeyType::KEY_ESC;
      break;
    case CACA_KEY_UP:
      keyPressed = Arcade::KeyType::KEY_UP;
      break;
    case CACA_KEY_DOWN:
      keyPressed = Arcade::KeyType::KEY_DOWN;
      break;
    case CACA_KEY_RIGHT:
      keyPressed = Arcade::KeyType::KEY_RIGHT;
      break;
    case CACA_KEY_LEFT:
      keyPressed = Arcade::KeyType::KEY_LEFT;
      break;
    case CACA_KEY_RETURN:
      keyPressed = Arcade::KeyType::KEY_ENTER;
      break;
    default:
      keyPressed = Arcade::KeyType::KEY_UNKNOWN;
      break;
  }
  _callback({
    Arcade::EventType::KEY_PRESSED,
    keyPressed,
    key
  });
}
