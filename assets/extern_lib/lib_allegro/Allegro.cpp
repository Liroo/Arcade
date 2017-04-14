#include <iostream>
#include <cctype>

#include "Allegro.h"
#include "exceptions/ArcadeException.hpp"

Allegro::Allegro() {}
Allegro::~Allegro() {}

void Allegro::init(const Arcade::Callback& callback) {
  // init allegro and custom attributes
  if (!al_init()
    || !(_timer = al_create_timer(TICK_MS))
    || !al_init_font_addon()
    || !al_init_ttf_addon()
    || !al_init_image_addon()
    || !al_init_primitives_addon()
    || !al_install_keyboard()
    || !(_eventQueue = al_create_event_queue())
    || !(_keyboardEventSource = al_get_keyboard_event_source())) {
    throw Arcade::Exception::ArcadeException(ERR_GRAPHICINIT);
  }
  _widthScreen = DSPL_WIDTH;
  _heightScreen = DSPL_HEIGHT;
  al_register_event_source(_eventQueue, _keyboardEventSource);
  al_register_event_source(_eventQueue, al_get_timer_event_source(_timer));
  _callback = callback;
  _isDrawing = false;
  _isLooping = false;
}

void Allegro::run() {
  if (!(_display = al_create_display(DSPL_WIDTH, DSPL_HEIGHT))) {
    throw Arcade::Exception::ArcadeException(ERR_GRAPHICINIT);
  }
  // loopinp
  al_clear_to_color(al_map_rgb(0,0,0));
  al_flip_display();
  al_start_timer(_timer);
  _isRunning = true;
  _isLooping = true;
  _callback({
    Arcade::EventType::RESIZE,
    Arcade::KeyType::KEY_UNKNOWN,
    0
  });
  while (doesLooping()) {
    // event
    ALLEGRO_EVENT event;
    al_get_next_event(_eventQueue, &event);
    if (event.type == ALLEGRO_EVENT_TIMER) {
      _callback({
        Arcade::EventType::TICK,
        Arcade::KeyType::KEY_UNKNOWN,
        0
      });
    } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
      _handleEvent(event);
    }
  }
  al_stop_timer(_timer);
  al_flush_event_queue(_eventQueue);
  if (_eventQueue && _keyboardEventSource) {
    al_destroy_event_queue(_eventQueue);
  }
  if (_display) {
    al_destroy_display(_display);
  }
  for (auto it = _imageCache.begin(); it < _imageCache.end(); it++) {
    al_destroy_bitmap((*it).image);
  }
  _isLooping = false;
}

void Allegro::close() {
  if (_eventQueue && _keyboardEventSource) {
    al_unregister_event_source(_eventQueue, _keyboardEventSource);
  }
  al_unregister_event_source(_eventQueue, al_get_timer_event_source(_timer));
  if (_eventQueue) {
    al_flush_event_queue(_eventQueue);
  }
  _isRunning = false;
}

void Allegro::update(Arcade::ObjectList objs) {
  if (!_isRunning || !_isLooping) {
    return;
  }
  _isDrawing = true;
  std::sort(objs.begin(), objs.end(), [](Arcade::Object i1, Arcade::Object i2) -> bool {
    return i1.elevation < i2.elevation;
  });
  std::for_each(objs.begin(), objs.end(), [=](Arcade::Object obj) {
    if (obj.elevation > 0) {
      _drawObj(obj);
    }
  });
  al_flip_display();
  _isDrawing = false;
}

bool Allegro::isRunning() const {
  return _isRunning || _isDrawing || _isLooping;
}

bool Allegro::isDeletable() const {
  return !(_isDrawing || (_isLooping || _isRunning));
}

bool Allegro::isClosed() const {
  return !_isLooping && !_isDrawing;
}

bool Allegro::doesLooping() const {
  return _isRunning || _isDrawing;
}

void Allegro::_handleEvent(const ALLEGRO_EVENT& event) {
  if (!_isRunning) {
    return;
  }
  Arcade::KeyType keyPressed = Arcade::KeyType::KEY_UNKNOWN;

  // event serialization
  switch (event.keyboard.keycode) {
    case ALLEGRO_KEY_2:
      keyPressed = Arcade::KeyType::KEY_TWO;
      break;
    case ALLEGRO_KEY_3:
      keyPressed = Arcade::KeyType::KEY_THREE;
      break;
    case ALLEGRO_KEY_4:
      keyPressed = Arcade::KeyType::KEY_FOUR;
      break;
    case ALLEGRO_KEY_5:
      keyPressed = Arcade::KeyType::KEY_FIVE;
      break;
    case ALLEGRO_KEY_8:
      keyPressed = Arcade::KeyType::KEY_EIGHT;
      break;
    case ALLEGRO_KEY_9:
      keyPressed = Arcade::KeyType::KEY_NINE;
      break;
    case ALLEGRO_KEY_DOWN:
      keyPressed = Arcade::KeyType::KEY_DOWN;
      break;
    case ALLEGRO_KEY_UP:
      keyPressed = Arcade::KeyType::KEY_UP;
      break;
    case ALLEGRO_KEY_RIGHT:
      keyPressed = Arcade::KeyType::KEY_RIGHT;
      break;
    case ALLEGRO_KEY_LEFT:
      keyPressed = Arcade::KeyType::KEY_LEFT;
      break;
    case ALLEGRO_KEY_ESCAPE:
      keyPressed = Arcade::KeyType::KEY_ESC;
      break;
    case ALLEGRO_KEY_ENTER:
      keyPressed = Arcade::KeyType::KEY_ENTER;
      break;
    default:
      keyPressed = Arcade::KeyType::KEY_UNKNOWN;
      break;
  }
  _callback({
    Arcade::EventType::KEY_PRESSED,
    keyPressed,
    event.keyboard.keycode
  });
}

void Allegro::_drawObj(const Arcade::Object& obj) {
  if (!_isRunning) {
    return;
  }

  if (obj.backgroundColor >= 0 && obj.imageName.empty())
    _drawButton(obj);
  if (!obj.imageName.empty())
    _drawImage(obj);
  if (!obj.text.empty())
    _drawText(obj);
}

void Allegro::_drawText(const Arcade::Object& obj) {
  ALLEGRO_FONT *font = al_load_font(FONT_PATH, obj.fontSize, 0);
  int lineCount = std::count(obj.text.begin(), obj.text.end(), '\n');

  if (lineCount > 0) {
    al_draw_multiline_text(font, al_map_rgb(255, 255, 255),
      obj.position.first + 10,
      obj.position.second + 10,
      obj.size.first, obj.size.second / lineCount,
      ALLEGRO_ALIGN_LEFT, obj.text.c_str());
  } else {
    al_draw_text(font, al_map_rgb(255, 255, 255),
      obj.position.first + (obj.size.first / 2),
      obj.position.second + (obj.size.second / 2) - (obj.fontSize / 1.7),
      ALLEGRO_ALIGN_CENTRE, obj.text.c_str());
  }
  al_destroy_font(font);
}

void Allegro::_drawButton(const Arcade::Object& obj) {
  int red = (obj.backgroundColor & 0xff0000) >> 16;
  int green = (obj.backgroundColor & 0x00ff00) >> 8;
  int blue = (obj.backgroundColor & 0x0000ff);

  al_draw_filled_rectangle(obj.position.first,
    obj.position.second, obj.position.first + obj.size.first,
    obj.position.second + obj.size.second,
    al_map_rgb(red, green, blue));
}

void Allegro::_drawImage(const Arcade::Object& obj) {
  ImageCacheSystem cacheItem = _getImageFromCache(obj);
  ALLEGRO_BITMAP* image = cacheItem.image;

  if (!image) {
    return;
  }

  float w = al_get_bitmap_width(image);
  float h = al_get_bitmap_height(image);
  al_draw_scaled_rotated_bitmap(image,
    w / 2, h / 2,
    obj.position.first + obj.size.first/2, obj.position.second + obj.size.second/2,
    (float)obj.size.first / w,
    (float)obj.size.second / h,
    obj.imageRotation, 0);
  if (!cacheItem.isCached) {
    al_destroy_bitmap(image);
  }
}

Allegro::ImageCacheSystem Allegro::_getImageFromCache(const Arcade::Object& obj) {
  std::string idTrimed = obj.id;
  if (isdigit(idTrimed[idTrimed.size() - 1])) {
    idTrimed.erase(idTrimed.find_last_of(':'), idTrimed.size() - 1);
    auto it = std::find_if(_imageCache.begin(), _imageCache.end(),
      [=](ImageCacheSystem item) -> bool {
        if (idTrimed == item.id) {
          return true;
        }
        return false;
      });
    if (it == _imageCache.end()) {
      ImageCacheSystem newCache = {
        idTrimed,
        al_load_bitmap(obj.imageName.c_str()),
        true
      };
       _imageCache.push_back(newCache);
       return newCache;
    } else {
      return (*it);
    }
  }
  return {
    "",
    al_load_bitmap(obj.imageName.c_str()),
    false
  };
}
