#ifndef OBJECT_H
# define OBJECT_H

# include <string>
# include <vector>

namespace Arcade {
  /*
    Raw image that is used to interface games and graphics lib
  */
  typedef struct s_object {
    /*
      positionFrom:
        usefull to create animation from a position to another
    */
    std::pair<int, int> positionFrom;

    /*
      positionTo:
      usefull to create animation to a position from another
    */
    std::pair<int, int> positionTo;

    /*
      animationDuration:
      set the animation duration
    */
    double animationDuration;

    /*
      animationType:
      the type of animation, could be fade, instant, elastic, etc...
    */
    std::string animationType;

    /*
      text:
      text to display at a certain position
    */
    std::string text;

    /*
      imageName:
      image to extract and to display at a certain position
    */
    std::string imageName;

    /*
      rawImage:
      if graphic library is ascii mode, use rawImage as image
    */
    std::vector<std::vector<char>> rawImage;

    /*
      id to know which object it is
    */
    std::string id;

    /*
      elevation, order to display thing
      or -1 to hide the object
    */
    int elevation;
  } Object;

  typedef std::vector<Object> ObjectList;
};

#endif
