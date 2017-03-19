#ifndef OBJECT_HPP
# define OBJECT_HPP

# include <string>

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
  } Object;
}

#endif
