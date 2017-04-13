#ifndef CACA_H
# define CACA_H

# include "IGraphic.h"
# include <caca.h>

# define DSPL_WIDTH 128
# define DSPL_HEIGHT 72

class Caca: public Arcade::IGraphic {
  public:
    Caca();
    virtual ~Caca();

  public:
    virtual void init(const Arcade::Callback&);
    virtual void run();
    virtual void close();
    virtual void update(Arcade::ObjectList);

    virtual bool isRunning() const;
    virtual bool isDeletable() const;
    virtual bool isClosed() const;

  private:
    Arcade::Callback _callback;

  private:
    bool _isRunning;

  private:
    caca_display_t* _display;
    caca_canvas_t* _canvas;

};


extern "C" {
  Arcade::IGraphic* load_lib() {
    return new Caca;
  }
}


#endif
