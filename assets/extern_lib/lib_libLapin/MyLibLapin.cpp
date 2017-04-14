#include <iostream>
#include <algorithm>
#include "MyLibLapin.h"
#include "exceptions/ArcadeException.hpp"

Arcade::Callback Arcade::MyLibLapin::callbackFunction; // init static member variable

Arcade::MyLibLapin::MyLibLapin() {}

Arcade::MyLibLapin::~MyLibLapin() {}

/* Init Window and run mainloop */
void Arcade::MyLibLapin::init(const Arcade::Callback& tmp) {
  Arcade::MyLibLapin::callbackFunction = tmp; // init static member variable

  _window = bunny_start(1281, 721, false, "LibLapin Window"); // Create window
  if (!_window) {
    throw Arcade::Exception::ArcadeException(ERR_GRAPHICINIT);
  }
  _initFontMap();

  bunny_set_key_response(key_response); /* lapin/events.h */
  bunny_set_loop_main_function(main_loop);
  bunny_fill(&_window->buffer, BLACK);
}

bool Arcade::MyLibLapin::isRunning() const {
  return _isLooping || !_isWorking;
}

bool Arcade::MyLibLapin::isDeletable() const {
  return !_isLooping && !_isWorking;
}

bool Arcade::MyLibLapin::isClosed() const {
  return !_isLooping;
}

void Arcade::MyLibLapin::run() {
  _isLooping = true;
  bunny_loop(_window, 10, this);
  bunny_stop(_window);
  for (auto it = _cache.begin(); it != _cache.end(); ++it) {
    bunny_delete_clipable(it->second);
  }
}

void Arcade::MyLibLapin::close() {
  _isLooping = false;
}

void Arcade::MyLibLapin::update(std::vector<Arcade::Object> objs) {

  if (_isLooping) {
    _isWorking = true;
    bunny_fill(&_window->buffer, BLACK);
    std::sort(objs.begin(), objs.end(), [](Arcade::Object i1, Arcade::Object i2) -> bool {
      return i1.elevation < i2.elevation;
    });
    std::for_each(objs.begin(), objs.end(), [=](Arcade::Object obj) {
      _drawObj(obj);
    });
    if (_isLooping) {
      bunny_display(_window); // compute every drawing made on the window and display it on screen
    }
    _isWorking = false;
  }
}

void Arcade::MyLibLapin::_drawObj(const Arcade::Object& obj) {

  if (obj.backgroundColor >= 0)
    _drawButton(obj);
  if (!obj.imageName.empty())
    _drawImage(obj);
  if (!obj.text.empty())
    _drawText(obj);
}

bool Arcade::MyLibLapin::_checkIfN(const std::string& str) {

  for (std::string::size_type i = 0; i < str.length(); ++i) {
    if (str[i] == '\n')
      return true;
  }
  return false;
}

void Arcade::MyLibLapin::_drawText(const Arcade::Object& obj) {

  t_bunny_position pos;
  int backspace = obj.position.second + 10;
  int posAbs = -1;
  int posX = obj.position.first + ((obj.size.first - (obj.text.length() * (obj.fontSize - (obj.fontSize / 2)))) / 2);

  if (_checkIfN(obj.text))
    posX = obj.position.first + 10;

  for(std::string::size_type i = 0; i < obj.text.length(); ++i) {

      char c = obj.text[i];
      posAbs++;
      if (c >= 65 && c <= 90)
        c = c + 32;
      if (c == '\n') {
        backspace += 30;
        posAbs = -1;
        continue;
      }
      if (c == ' ') {
        continue;
      }

      std::map<char, t_bunny_picture*>::iterator it = _fontMap.find(c);
      t_bunny_picture *letter = it->second;

      pos.x = posX + (posAbs * (obj.fontSize - (obj.fontSize / 2)));
      pos.y = (c == '_') ? (backspace + 8) : backspace;

      /* Resize the picture */
      if (c != '_')
        letter->scale.y = (float)((float)(obj.fontSize - (obj.fontSize / 2)) / (float)letter->clip_height);
      letter->scale.x = (float)((float)(obj.fontSize - (obj.fontSize / 2)) / (float)letter->clip_width);

      if (_isLooping) {
        bunny_blit(&_window->buffer, letter, &pos);
      }
  }
}

void Arcade::MyLibLapin::_drawButton(const Arcade::Object& obj) {

  t_bunny_picture *pic; // Picture
  t_bunny_position pos; // Picture's position

  int isSameCount = std::count(obj.id.begin(), obj.id.end(), ':');
  try {
    if (isSameCount == 2) {
      pic = _cache.at(obj.id.substr(0, obj.id.find_last_of(':')));
    } else {
      pic = _cache.at(obj.id);
    }
  } catch (std::out_of_range &e) {
    (void)e;
    pic = bunny_new_picture(obj.size.first, obj.size.second); // Load picture
    if (isSameCount == 2) {
      _cache[obj.id.substr(0, obj.id.find_last_of(':'))] = pic;
    } else {
      _cache[obj.id] = pic;
    }
  }

  pos.x = obj.position.first;
  pos.y = obj.position.second;

  int r = (obj.backgroundColor & 0xff0000) >> 16;
  int g = (obj.backgroundColor & 0x00ff00) >> 8;
  int b = (obj.backgroundColor & 0x0000ff);

  bunny_fill(&pic->buffer, COLOR(255, r, g, b));

  /* Draw the picture on screen at the sent position */
  if (_isLooping) {
    bunny_blit(&_window->buffer, pic, &pos);
  }
}

void Arcade::MyLibLapin::_drawImage(const Arcade::Object& obj) {
  t_bunny_picture *pic; // Picture
  t_bunny_position pos; // Picture's position

  int isSameCount = std::count(obj.id.begin(), obj.id.end(), ':');
  try {
    if (isSameCount == 2) {
      pic = _cache.at(obj.id.substr(0, obj.id.find_last_of(':')));
    } else {
      pic = _cache.at(obj.id);
    }
  } catch (std::out_of_range &e) {
    (void)e;
    pic = bunny_load_picture(obj.imageName.c_str()); // Load picture
    if (isSameCount == 2) {
      _cache[obj.id.substr(0, obj.id.find_last_of(':'))] = pic;
    } else {
      _cache[obj.id] = pic;
    }
  }
  if (!pic) {
    return;
  }

  pos.x = obj.position.first;
  pos.y = obj.position.second;

  /* Resize the picture */
  pic->scale.x = (float)((float)obj.size.first / (float)pic->clip_width);
  pic->scale.y = (float)((float)obj.size.second / (float)pic->clip_height);

  /* rotate */
  pic->rotation = obj.imageRotation;

  /* Draw the picture on screen at the sent position */
  if (_isLooping) {
    bunny_blit(&_window->buffer, pic, &pos);
  }
}

Arcade::Event Arcade::MyLibLapin::eventAssign(Arcade::EventType event, KeyType key, int value) {

  Arcade::Event tmp;

  tmp.type = event;
  tmp.key = key;
  tmp.data = value;
  return (tmp);
}

t_bunny_response	Arcade::MyLibLapin::main_loop(void *data) {
  // Send Tick to Core
  Arcade::MyLibLapin::callbackFunction(Arcade::MyLibLapin::eventAssign(TICK, KEY_UNKNOWN, 0));
  if (!((MyLibLapin*)data)->isClosed()) {
    return (GO_ON);
  }
  return (EXIT_ON_SUCCESS);
}

t_bunny_response	Arcade::MyLibLapin::key_response(t_bunny_event_state state, t_bunny_keysym sym, void *data) {

  (void)data;
  Arcade::KeyType keyPressed = Arcade::KeyType::KEY_UNKNOWN;

  // event serialization
  if (state == GO_UP) {

    switch (sym) {
      case BKS_2:
        keyPressed = Arcade::KeyType::KEY_TWO;
        break;
      case BKS_3:
        keyPressed = Arcade::KeyType::KEY_THREE;
        break;
      case BKS_4:
        keyPressed = Arcade::KeyType::KEY_FOUR;
        break;
      case BKS_5:
        keyPressed = Arcade::KeyType::KEY_FIVE;
        break;
      case BKS_8:
        keyPressed = Arcade::KeyType::KEY_EIGHT;
        break;
      case BKS_9:
        keyPressed = Arcade::KeyType::KEY_NINE;
        break;
      case BKS_DOWN:
        keyPressed = Arcade::KeyType::KEY_DOWN;
        break;
      case BKS_UP:
        keyPressed = Arcade::KeyType::KEY_UP;
        break;
      case BKS_RIGHT:
        keyPressed = Arcade::KeyType::KEY_RIGHT;
        break;
      case BKS_LEFT:
        keyPressed = Arcade::KeyType::KEY_LEFT;
        break;
      case BKS_ESCAPE:
        keyPressed = Arcade::KeyType::KEY_ESC;
        break;
      case BKS_RETURN:
        keyPressed = Arcade::KeyType::KEY_ENTER;
        break;
      case BKS_BACKSPACE:
        sym = (t_bunny_keysym)62;
        break;
      default:
        keyPressed = Arcade::KeyType::KEY_UNKNOWN;
        break;
    }
    callbackFunction({
      Arcade::EventType::KEY_PRESSED,
      keyPressed,
      sym + 1
    });
  }

return (GO_ON);
}

void Arcade::MyLibLapin::_initFontMap() {

  std::string allFont("abcdefghijklmnopqrstuvwxyz0123456789");
  for(std::string::size_type i = 0; i < allFont.length(); ++i) {

      char c = allFont[i];
      _initLetter(c);
  }
  std::string specialFont(".:<>");
  for(std::string::size_type i = 0; i < specialFont.length(); ++i) {

      char c = specialFont[i];
      _initSpecialCaracter(c);
  }
  _initUnderscore();
}

void Arcade::MyLibLapin::_deleteFontMap() {

  std::map<char, t_bunny_picture*>::iterator it = _fontMap.begin();
  for (it=_fontMap.begin(); it!=_fontMap.end(); ++it) {
    bunny_delete_clipable(it->second);
  }
}

void Arcade::MyLibLapin::_initLetter(char c) {

  t_bunny_picture *pic; // Picture
  pic = bunny_load_picture(FONT_PATH); // Load picture

  switch (c) {
    case 'a':
    case 'A':
      pic->clip_x_position = 0;
      pic->clip_y_position = 0;
      pic->clip_width = 28;
      pic->clip_height = 60;
      break;

    case 'b':
    case 'B':
      pic->clip_x_position = 28;
      pic->clip_y_position = 0;
      pic->clip_width = 28;
      pic->clip_height = 60;
      break;

    case 'c':
    case 'C':
      pic->clip_x_position = 56;
      pic->clip_y_position = 0;
      pic->clip_width = 28;
      pic->clip_height = 60;
      break;

    case 'd':
    case 'D':
      pic->clip_x_position = 84;
      pic->clip_y_position = 0;
      pic->clip_height = 60;
      pic->clip_width = 28;
      break;

    case 'e':
    case 'E':
      pic->clip_x_position = 112;
      pic->clip_y_position = 0;
      pic->clip_width = 28;
      pic->clip_height = 60;
      break;

    case 'f':
    case 'F':
      pic->clip_x_position = 140;
      pic->clip_y_position = 0;
      pic->clip_width = 20;
      pic->clip_height = 60;
      break;

    case 'g':
    case 'G':
      pic->clip_x_position = 158;
      pic->clip_y_position = 0;
      pic->clip_width = 28;
      pic->clip_height = 70;
      break;

    case 'h':
    case 'H':
      pic->clip_x_position = 186;
      pic->clip_y_position = 0;
      pic->clip_width = 28;
      pic->clip_height = 60;
      break;

    case 'i':
    case 'I':
      pic->clip_x_position = 214;
      pic->clip_y_position = 0;
      pic->clip_width = 18;
      pic->clip_height = 60;
      break;

    case 'j':
    case 'J':
      pic->clip_x_position = 232;
      pic->clip_y_position = 0;
      pic->clip_width = 18;
      pic->clip_height = 65;
      break;

    case 'k':
    case 'K':
      pic->clip_x_position = 250;
      pic->clip_y_position = 0;
      pic->clip_width = 28;
      pic->clip_height = 60;
      break;

    case 'l':
    case 'L':
      pic->clip_x_position = 274;
      pic->clip_y_position = 0;
      pic->clip_height = 60;
      pic->clip_width = 18;
      break;

    case 'm':
    case 'M':
      pic->clip_x_position = 296;
      pic->clip_y_position = 0;
      pic->clip_height = 60;
      pic->clip_width = 34;
      break;

    case 'n':
    case 'N':
      pic->clip_x_position = 332;
      pic->clip_y_position = 0;
      pic->clip_height = 60;
      pic->clip_width = 28;
      break;

    case 'o':
    case 'O':
      pic->clip_x_position = 360;
      pic->clip_y_position = 0;
      pic->clip_height = 60;
      pic->clip_width = 28;
      break;

    case 'p':
    case 'P':
      pic->clip_x_position = 390;
      pic->clip_y_position = 0;
      pic->clip_height = 66;
      pic->clip_width = 28;
      break;

    case 'q':
    case 'Q':
      pic->clip_x_position = 418;
      pic->clip_y_position = 0;
      pic->clip_height = 66;
      pic->clip_width = 28;
      break;

    case 'r':
    case 'R':
      pic->clip_x_position = 446;
      pic->clip_y_position = 0;
      pic->clip_height = 60;
      pic->clip_width = 28;
      break;

    case 's':
    case 'S':
      pic->clip_x_position = 470;
      pic->clip_y_position = 0;
      pic->clip_width = 28;
      pic->clip_height = 60;
      break;

    case 't':
    case 'T':
      pic->clip_x_position = 500;
      pic->clip_y_position = 0;
      pic->clip_height = 60;
      pic->clip_width = 18;
      break;

    case 'u':
    case 'U':
      pic->clip_x_position = 518;
      pic->clip_y_position = 0;
      pic->clip_width = 28;
      pic->clip_height = 60;
      break;

    case 'v':
    case 'V':
      pic->clip_x_position = 540;
      pic->clip_y_position = 0;
      pic->clip_width = 28;
      pic->clip_height = 60;
      break;

    case 'w':
    case 'W':
      pic->clip_x_position = 567;
      pic->clip_y_position = 0;
      pic->clip_width = 38;
      pic->clip_height = 60;
      break;

    case 'x':
    case 'X':
      pic->clip_x_position = 602;
      pic->clip_y_position = 0;
      pic->clip_height = 60;
      pic->clip_width = 26;
      break;

    case 'y':
    case 'Y':
      pic->clip_x_position = 626;
      pic->clip_y_position = 0;
      pic->clip_height = 66;
      pic->clip_width = 28;
      break;

    case 'z':
    case 'Z':
      pic->clip_x_position = 650;
      pic->clip_y_position = 0;
      pic->clip_height = 60;
      pic->clip_width = 28;
      break;

    case '0':
      pic->clip_x_position = 0;
      pic->clip_y_position = 140;
      pic->clip_height = 60;
      pic->clip_width = 28;
      break;

    case '1':
      pic->clip_y_position = 140;
      pic->clip_x_position = 28;
      pic->clip_height = 60;
      pic->clip_width = 20;
      break;

    case '2':
      pic->clip_x_position = 42;
      pic->clip_y_position = 140;
      pic->clip_height = 60;
      pic->clip_width = 28;
      break;

    case '3':
      pic->clip_x_position = 70;
      pic->clip_y_position = 140;
      pic->clip_height = 60;
      pic->clip_width = 28;
      break;

    case '4':
      pic->clip_x_position = 100;
      pic->clip_y_position = 140;
      pic->clip_height = 60;
      pic->clip_width = 28;
      break;

    case '5':
      pic->clip_x_position = 130;
      pic->clip_y_position = 140;
      pic->clip_height = 60;
      pic->clip_width = 28;
      break;

    case '6':
      pic->clip_x_position = 160;
      pic->clip_y_position = 140;
      pic->clip_height = 60;
      pic->clip_width = 28;
      break;

    case '7':
      pic->clip_x_position = 185;
      pic->clip_y_position = 140;
      pic->clip_height = 60;
      pic->clip_width = 28;
      break;

    case '8':
      pic->clip_x_position = 210;
      pic->clip_y_position = 140;
      pic->clip_height = 60;
      pic->clip_width = 28;
      break;

    case '9':
      pic->clip_x_position = 240;
      pic->clip_y_position = 140;
      pic->clip_height = 60;
      pic->clip_width = 28;
      break;

  }

  _fontMap.insert(std::make_pair(c, pic));
}

void Arcade::MyLibLapin::_initSpecialCaracter(char c) {

  t_bunny_picture *pic; // Picture
  pic = bunny_load_picture(FONT_SPECIALPATH); // Load picture

  switch (c) {

    case '.':
      pic->clip_x_position = 100;
      pic->clip_y_position = 475;
      pic->clip_width = 28;
      pic->clip_height = 60;
      break;

    case ':':
      pic->clip_x_position = 240;
      pic->clip_y_position = 475;
      pic->clip_width = 28;
      pic->clip_height = 60;
      break;

    case '<':
      pic->clip_x_position = 820;
      pic->clip_y_position = 475;
      pic->clip_width = 28;
      pic->clip_height = 60;
      break;

    case '>':
      pic->clip_x_position = 890;
      pic->clip_y_position = 475;
      pic->clip_width = 28;
      pic->clip_height = 60;
      break;
  }

  _fontMap.insert(std::make_pair(c, pic));

}

void Arcade::MyLibLapin::_initUnderscore() {

  t_bunny_picture *pic; // Picture

  pic = bunny_new_picture(30, 1); // Load picture
  bunny_fill(&pic->buffer, BLACK);
  _fontMap.insert(std::make_pair('_', pic));
}
