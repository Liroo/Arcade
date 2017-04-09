#ifndef ALLEGRO_H
# define ALLEGRO_H

# include <memory>
# include <algorithm>
# include "IGraphic.h"
# include "allegro5/allegro.h"
# include "allegro5/allegro_ttf.h"
# include "allegro5/allegro_font.h"
# include "allegro5/allegro_primitives.h"
# include "allegro5/allegro_image.h"

class Allegro: public Arcade::IGraphic {
  public:
    Allegro();
    virtual ~Allegro();

  public:
    virtual void init(const Arcade::Callback&);
    virtual void close();
    virtual void update(std::vector<Arcade::Object>);
    void drawObj(const Arcade::Object&) const;
    void drawText(const Arcade::Object&) const;
    void drawButton(const Arcade::Object&) const;
    void drawImage(const Arcade::Object&) const;

  private:
    Arcade::Callback _callback;
    bool _isRunning;
    int _widthScreen;
    int _heightScreen;

  private:
    // specific to the library Allegro
    ALLEGRO_DISPLAY* _display;
    ALLEGRO_EVENT_QUEUE* _eventQueue;
    ALLEGRO_EVENT_SOURCE* _keyboardEventSource;
};


extern "C" {
  Arcade::IGraphic* load_lib() {
    return new Allegro;
  }
}


#endif
