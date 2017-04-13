#include <iostream>
#include <algorithm>
#include "MyLibLapin.h"

  Callback Arcade::MyLibLapin::callbackFunction; // init static member variable

  MyLibLapin::MyLibLapin() {

    _initFontMap();
  }

  MyLibLapin::~MyLibLapin() {}

  /* Init Window and run mainloop */
  void MyLibLapin::init(const Callback& tmp) {

    Arcade::MyLibLapin::callbackFunction = tmp; // Callback to Core

    _window = bunny_start(1281, 721, false, "LibLapin Window"); // Create window

    bunny_set_key_response(key_response); /* lapin/events.h */
    bunny_set_loop_main_function(main_loop);
    bunny_fill(&_window->buffer, BLACK);
  }

  bool MyLibLapin::isRunning() const {
    return _isLooping;
  }

  bool MyLibLapin::isDeletable() const {
    return !_isLooping;
  }

  bool MyLibLapin::isClosed() const {
    return _isLooping;
  }

  void MyLibLapin::run() {

    _isLooping = true;
    bunny_loop(_window, 10, this);
    _isLooping = false;
  }

  void MyLibLapin::close() {

    bunny_stop(_window);
  }

  void MyLibLapin::update(std::vector<Arcade::Object> objs) {

    if (_isLooping) {

      bunny_fill(&_window->buffer, BLACK);
      std::sort(objs.begin(), objs.end(), [](Arcade::Object i1, Arcade::Object i2) -> bool {
        return i1.elevation < i2.elevation;
      });
      std::for_each(objs.begin(), objs.end(), [=](Arcade::Object obj) {
        _drawObj(obj);
      });
      bunny_display(_window); // compute every drawing made on the window and display it on screen
    }
  }

  void MyLibLapin::_drawObj(const Arcade::Object& obj) {

    if (obj.backgroundColor >= 0)
      _drawButton(obj);
    if (!obj.imageName.empty())
      _drawImage(obj);
    if (!obj.text.empty())
      _drawText(obj);
  }

  bool MyLibLapin::_checkIfN(const std::string& str) {

    for (std::string::size_type i = 0; i < str.length(); ++i) {
      if (str[i] == '\n')
        return true;
    }
    return false;
  }

  void MyLibLapin::_drawText(const Arcade::Object& obj) {

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

        bunny_blit(&_window->buffer, letter, &pos);
    }
  }

  void MyLibLapin::_drawButton(const Arcade::Object& obj) {

    t_bunny_picture *pic; // Picture
    t_bunny_position pos; // Picture's position

    pic = bunny_new_picture(obj.size.first, obj.size.second); // Load picture

    pos.x = obj.position.first;
    pos.y = obj.position.second;

    int r = (obj.backgroundColor & 0xff0000) >> 16;
    int g = (obj.backgroundColor & 0x00ff00) >> 8;
    int b = (obj.backgroundColor & 0x0000ff);

    bunny_fill(&pic->buffer, COLOR(255, r, g, b));

    /* Draw the picture on screen at the sent position */
    bunny_blit(&_window->buffer, pic, &pos);
    bunny_delete_clipable(pic);

  }

  void MyLibLapin::_drawImage(const Arcade::Object& obj) {

    t_bunny_picture *pic; // Picture
    t_bunny_position pos; // Picture's position

    pic = bunny_load_picture(obj.imageName.c_str()); // Load picture

    pos.x = obj.position.first;
    pos.y = obj.position.second;

    /* Resize the picture */
    pic->scale.x = (float)((float)obj.size.first / (float)pic->clip_width);
    pic->scale.y = (float)((float)obj.size.second / (float)pic->clip_height);

    /* rotate */
    pic->rotation = obj.imageRotation;

    /* Draw the picture on screen at the sent position */
    bunny_blit(&_window->buffer, pic, &pos);
    bunny_delete_clipable(pic);

  }

  Event MyLibLapin::eventAssign(EventType event, KeyType key, int value) {

    Event tmp;

    tmp.type = event;
    tmp.key = key;
    tmp.data = value;
    return (tmp);
  }

  t_bunny_response	MyLibLapin::main_loop(void *data) {

    (void)data;

    // Send Tick to Core
    Arcade::MyLibLapin::callbackFunction(Arcade::MyLibLapin::eventAssign(TICK, KEY_UNKNOWN, 0));
    return (GO_ON);
  }

  t_bunny_response	MyLibLapin::key_response(t_bunny_event_state state, t_bunny_keysym sym, void *data) {

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
        default:
          break;
      }
      callbackFunction({
        Arcade::EventType::KEY_PRESSED,
        keyPressed,
        0
      });
    }

  return (GO_ON);
  }

  void MyLibLapin::_initFontMap() {

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

  void MyLibLapin::_deleteFontMap() {

    std::map<char, t_bunny_picture*>::iterator it = _fontMap.begin();
    for (it=_fontMap.begin(); it!=_fontMap.end(); ++it) {

      std::cout << it->first << std::endl;
      bunny_delete_clipable(it->second);
      std::cout << it->first << std::endl;
    }
  }

  void MyLibLapin::_initLetter(char c) {

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

  void MyLibLapin::_initSpecialCaracter(char c) {

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

  void MyLibLapin::_initUnderscore() {

    t_bunny_picture *pic; // Picture

    pic = bunny_new_picture(30, 1); // Load picture
    bunny_fill(&pic->buffer, BLACK);
    _fontMap.insert(std::make_pair('_', pic));
  }

}
