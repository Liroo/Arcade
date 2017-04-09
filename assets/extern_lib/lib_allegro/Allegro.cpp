#include <iostream>
#include "Allegro.h"

Allegro::Allegro() {}
Allegro::~Allegro() {}

void Allegro::init(const Arcade::Callback& callback) {
  if (al_init() == false) {
    throw;
  }
  _widthScreen = 800;
  _heightScreen = 600;
  if (al_init_font_addon() == false) {
    throw;
  }
  if (al_init_ttf_addon() == false) {
    throw;
  }
  if (al_init_image_addon() == false) {
    throw;
  }
  if (al_install_keyboard() == false) {
    throw;
  }
  if (!(_display = al_create_display(800, 600))) {
    throw;
  }
  if (!(_eventQueue = al_create_event_queue())) {
    throw;
  }
  if (!(_keyboardEventSource = al_get_keyboard_event_source())) {
    throw;
  }
  if (al_init_primitives_addon() == false) {
    throw;
  }
  al_register_event_source(_eventQueue, _keyboardEventSource);
  al_clear_to_color(al_map_rgb_f(1, 1, 0));
  _isRunning = true;
  while (_isRunning == true)
  {
    al_flip_display();
    ALLEGRO_EVENT events;
    al_wait_for_event(_eventQueue, &events);
    if (events.type == ALLEGRO_EVENT_KEY_DOWN)
    {
      switch (events.keyboard.keycode)
      {
        case ALLEGRO_KEY_DOWN:
          callback({
            Arcade::EventType::KEY_PRESSED,
            Arcade::KeyType::KEY_DOWN,
            0
          });
          break;
        case ALLEGRO_KEY_UP:
          callback({
            Arcade::EventType::KEY_PRESSED,
            Arcade::KeyType::KEY_UP,
            0
          });
          break;
        case ALLEGRO_KEY_RIGHT:
          callback({
            Arcade::EventType::KEY_PRESSED,
            Arcade::KeyType::KEY_RIGHT,
            0
          });
          break;
        case ALLEGRO_KEY_LEFT:
          callback({
            Arcade::EventType::KEY_PRESSED,
            Arcade::KeyType::KEY_LEFT,
            0
          });
          break;
        case ALLEGRO_KEY_ESCAPE:
          callback({
            Arcade::EventType::KEY_PRESSED,
            Arcade::KeyType::KEY_ESC,
            0
          });
          break;
      }
    }
  }
}

void Allegro::close() {
  _isRunning = false;
  if (_eventQueue && _keyboardEventSource) {
    al_unregister_event_source(_eventQueue, _keyboardEventSource);
  }
  if (_display) {
    al_destroy_display(_display);
  }
}

void Allegro::update(Arcade::ObjectList objs) {
  std::sort(objs.begin(), objs.end(), [](Arcade::Object i1, Arcade::Object i2) -> bool {
    return i1.elevation < i2.elevation;
  });
  std::for_each(objs.begin(), objs.end(), [=](Arcade::Object obj) {
    drawObj(obj);
  });
  al_update_display_region(0, 0, _widthScreen, _heightScreen);
}

void Allegro::drawObj(const Arcade::Object& obj) const {
  if (obj.backgroundColor >= 0)
    drawButton(obj);
  if (obj.imageName.empty() == false)
    drawImage(obj);
  if (obj.text.empty() == false)
    drawText(obj);
}

void Allegro::drawText(const Arcade::Object& obj) const {
  ALLEGRO_FONT *font = al_load_font("../../ressource/font/SF.otf", 36, 0);
  al_draw_text(font, al_map_rgb(255, 255, 255), (_widthScreen * (obj.position.first + obj.size.first / 2)) / 10000, (_heightScreen * (obj.position.second + obj.size.second / 2)) / 10000 - 18, ALLEGRO_ALIGN_CENTRE, obj.text.c_str());
  al_destroy_font(font);
}

void Allegro::drawButton(const Arcade::Object& obj) const {
  int red = (obj.backgroundColor & 0xff0000) >> 16;
  int green = (obj.backgroundColor & 0x00ff00) >> 8;
  int blue = (obj.backgroundColor & 0x000000);
  al_draw_filled_rectangle((_widthScreen * obj.position.first) / 10000, (_heightScreen * obj.position.second) / 10000, (_widthScreen * (obj.position.first + obj.size.first)) / 10000, (_heightScreen * (obj.position.second + obj.size.second)) / 10000, al_map_rgb(red, green, blue));
  drawText(obj);
}

void Allegro::drawImage(const Arcade::Object& obj) const {
  ALLEGRO_BITMAP  *img = al_load_bitmap(obj.imageName.c_str());
  al_draw_scaled_bitmap(img, 0, 0, al_get_bitmap_width(img), al_get_bitmap_height(img), (_widthScreen * obj.position.first) / 10000, (_heightScreen * obj.position.second) / 10000, (_widthScreen * obj.size.first) / 10000, (_heightScreen * obj.size.second) / 10000, 0);
  al_flip_display();
  al_destroy_bitmap(img);
}
