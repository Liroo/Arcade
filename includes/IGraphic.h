#ifndef IGRAPHIC_H
# define IGRAPHIC_H

# include "Object.h"
# include "Event.h"

# define WIN_WIDTH 800
# define WIN_HEIGHT 600

namespace Arcade{

  class IGraphic{

    public:
      virtual ~IGraphic() {};

      virtual void init(const Callback&) = 0;
      virtual void close() = 0;
      virtual void update(std::vector<Arcade::Object>) = 0;

  };

}

#endif
