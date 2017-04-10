#ifndef OBJECT_H
# define OBJECT_H

# include <string>
# include <vector>
# include <algorithm>

namespace Arcade {
  /*
    Raw image that is used to interface games and graphics lib
  */
  typedef struct s_object {
    /*
      positionFrom:
        usefull to create animation from a position to another
    */
    std::pair<int, int> position = { 0, 0 };

    /*
      size:
      usefull to display with a correct size
    */
    std::pair<int, int> size = { 0, 0 };

    /*
      backgroundColor:
      background color
    */
    int backgroundColor = 0xFFFFFFFF;
    /*
      imageName:
      image to extract and to display at a certain position
    */
    std::string imageName = "";
    float imageRotation = 0.0;
    /*
      rawImage:
      if graphic library is ascii mode, use rawImage as image
    */
    std::vector<std::string> rawImage = {};
    /*
      text:
      text to display at a certain position
    */
    std::string text = "";
    int fontSize = 36;

    /*
      id to know which object it is
    */
    std::string id = "";

    /*
      elevation, order to display thing
      or -1 to hide the object
    */
    int elevation = -1;
  } Object;

  typedef std::vector<Object> ObjectList;
  inline ObjectList& appendObjectToList(ObjectList& ol, const Object& o) {
    auto it = std::find_if(ol.begin(), ol.end(), [&o](Object item) -> bool {
      if (item.id == o.id) { return true; }
      return false;
    });
    if (it != ol.end()) { ol.erase(it); }
    ol.push_back(o);
    return ol;
  }
  inline ObjectList& appendListToList(ObjectList& ol, const ObjectList& o) {
    for (auto it = o.begin(); it < o.end(); it++) {
      appendObjectToList(ol, *it);
    }
    return ol;
  }
};

#endif
