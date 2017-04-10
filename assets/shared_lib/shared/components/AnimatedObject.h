#ifndef ANIMATEDOBJECT_HPP
# define ANIMATEDOBJECT_HPP

# include <algorithm>
# include <ctime>
# include "DirectoryReader.h"
# include "Object.h"

namespace Arcade {
  class AnimatedObject {
    public:
      AnimatedObject(const std::string&,
        const std::string&);
      // size, position, id, directoryPath
      AnimatedObject(const std::pair<int, int>&,
        const std::pair<int, int>&,
        const std::string&,
        const std::string&);
      ~AnimatedObject();

    public:
      bool init();

    private:
      std::string _directoryPath;
      DirectoryReader::DirectoryContent _images;
      int _imagesIndex;

    private:
      Object _image;

    public:
      void setSize(const std::pair<int, int>&);
      void setPosition(const std::pair<int, int>&);

    public:
      Object render();
      Object renderNext(long);

  };
};

#endif
