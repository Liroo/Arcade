#ifndef MYLIBLAPIN_H
# define MYLIBLAPIN_H

# include <lapin.h>
# include <map>
# include "IGraphic.h"

# define FONT_PATH "assets/ressource/font/font.png"
# define FONT_SPECIALPATH "assets/ressource/font/specialFont.png"

namespace Arcade{

  /*
  ** Graphic class using MyLibLapin
  */
  class MyLibLapin : public IGraphic {

  public:
    MyLibLapin();
    virtual ~MyLibLapin();

  private:
    t_bunny_window *_window;
    bool _isLooping;
    std::map<char, t_bunny_picture*> _fontMap;

  private:
    void _initFontMap();
    void _initSpecialCaracter(char);
    void _initLetter(char);
    void _deleteFontMap();
    void _initUnderscore();
    bool _checkIfN(const std::string&);

  public:
    virtual void init(const Callback&); // Initializes and loops on the window
    virtual void close(); // Stop the mainloop
    virtual void update(std::vector<Arcade::Object>); // Update object on the window

    virtual bool isRunning() const;
    virtual bool isDeletable() const;
    virtual bool isClosed() const;
    virtual void run();

  bool _isWorking;

  public:
    static Callback callbackFunction; // Callback to the Core
    static Event eventAssign(EventType, KeyType, int); // Assign an Event
    static t_bunny_response key_response(t_bunny_event_state, t_bunny_keysym, void*); // Use to event key
    static t_bunny_response main_loop(void*); // timer and refresh

  private:
    void _drawObj(const Object& obj);
    void _drawText(const Object& obj);
    void _drawButton(const Object& obj);
    void _drawImage(const Object& obj);
    void _drawLetter(char, int, const Object);
    void _square(t_bunny_picture *, int, int, int, int, unsigned int);

  private:
    std::map<std::string, t_bunny_picture*> _cache;
  };

  extern "C" {
    Arcade::IGraphic* load_lib() {
      return new MyLibLapin;
    }

}

#endif
