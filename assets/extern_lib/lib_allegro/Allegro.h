#ifndef ALLEGRO_H
# define ALLEGRO_H

# include <memory>
# include "IGraphic.h"

class Allegro: public Arcade::IGraphic {
  public:
    Allegro();
    virtual ~Allegro();

  public:
    virtual void init(const Arcade::Callback&);
    virtual void close();
    virtual void update(std::vector<Arcade::Object>);

  private:
    Arcade::Callback _callback;
};

extern "C" {
  Arcade::IGraphic* load_lib() {
    return new Allegro;
  }
}

#endif
