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

    void _handleEvent(caca_event_t e);


  private:
    bool _isRunning;
    bool _isLooping;
    bool _isWorking;

  private:
    caca_display_t* _display;
    caca_canvas_t* _canvas;

  private:
    void _drawObj(Arcade::Object);

    void _drawButton(Arcade::Object);
    void _drawImage(Arcade::Object);
    void _drawText(Arcade::Object);

};


extern "C" {
  Arcade::IGraphic* load_lib() {
    return new Caca;
  }
}


#endif
