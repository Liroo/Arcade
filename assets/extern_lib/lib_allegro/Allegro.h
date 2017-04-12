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

# define FONT_PATH "assets/ressource/font/SF.otf"
# define TICK_MS (1.0/30) // 30 FPS
# define DSPL_WIDTH 1280
# define DSPL_HEIGHT 720

class Allegro: public Arcade::IGraphic {
  public:
    Allegro();
    virtual ~Allegro();

  public:
    virtual void init(const Arcade::Callback&);
    virtual void run();
    virtual void close();
    virtual void update(std::vector<Arcade::Object>);

    virtual bool isRunning() const;

    virtual bool isDeletable() const;
    virtual bool isClosed() const;
    bool doesLooping() const;

  private:
    bool _isDrawing;
    bool _isLooping;

  private:
    void _handleEvent(const ALLEGRO_EVENT&);

  private:
    void _drawObj(const Arcade::Object&);
    void _drawText(const Arcade::Object&);
    void _drawButton(const Arcade::Object&);
    void _drawImage(const Arcade::Object&);

  private:
    Arcade::Callback _callback;
    bool _isRunning;
    int _widthScreen;
    int _heightScreen;

  private:
    // specific to the library Allegro
    ALLEGRO_DISPLAY* _display;
    ALLEGRO_TIMER* _timer;
    ALLEGRO_EVENT_QUEUE* _eventQueue;
    ALLEGRO_EVENT_SOURCE* _keyboardEventSource;
};


extern "C" {
  Arcade::IGraphic* load_lib() {
    return new Allegro;
  }
}


#endif
