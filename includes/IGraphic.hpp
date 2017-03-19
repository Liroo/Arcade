#ifndef IGRAPHIC_HPP
# define IGRAPHIC_HPP

# include "Object.hpp"
# include "Event.hpp"

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
