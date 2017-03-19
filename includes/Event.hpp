#ifndef EVENT_HPP
# define EVENT_HPP

# include <pair>
# include <functional>

namespace Arcade {

  // refer to Event structure
  typedef enum e_eventType {
    RESIZE,
    KEY_PRESSED,
    UNKNOWN = -1
  } EventType;

  typedef struct s_event {
    // type event
    EventType type;

    // if type is resize, both of pair is used as window size
    // else, only first is the key pressed
    std::pair data;
  } Event;

  // Callback definition used for graphics library
  typedef std::function<void(const Event &)> Callback;
};

#endif
