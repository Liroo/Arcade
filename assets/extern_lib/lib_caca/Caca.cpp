#include <iostream>

#include "Caca.h"
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
  caca_set_display_time(_display, 10);
  caca_refresh_display(_display);
  while (isRunning()) {
    while (caca_get_event(_display, CACA_EVENT_ANY, &ev, 0)) {
      std::cout << "tick" << std::endl;
      if (caca_get_event_type(&ev) & CACA_EVENT_KEY_PRESS) {
        std::cout << caca_get_event_key_ch(&ev) << std::endl;
      }
    }
    caca_refresh_display(_display);
  }
  caca_free_display(_display);
}

void Caca::close() {
}

void Caca::update(Arcade::ObjectList objs) {
  (void)objs;
}

bool Caca::isRunning() const {
  return _isRunning;
}

bool Caca::isDeletable() const {
  return true;
}

bool Caca::isClosed() const {
  return true;
}
