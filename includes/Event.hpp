#ifndef EVENT_HPP
# define EVENT_HPP

# include <utility>
# include <functional>

namespace Arcade {

  typedef enum e_keyType {
    KEY_TWO, // Previous graphics library
    KEY_THREE, // Next graphics library
    KEY_FOUR, // Previous game
    KEY_FIVE, // Next game
    KEY_EIGHT, // Restart the game
    KEY_NINE, // Go back to the menu
    KEY_ESC, // Exit

    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,

    KEY_UNKNOWN = -1,
  } KeyType;

  // refer to Event structure
  typedef enum e_eventType {
    RESIZE,
    KEY_PRESSED,
    UNKNOWN = -1
  } EventType;

  typedef struct s_event {
    // type event
    // if RESIZE is called, dump should be called
    EventType type;
    KeyType keyType;

    // unknown key pressed
    int data;
  } Event;

  // Callback definition used for graphics library
  typedef std::function<void(const Event &)> Callback;
};

#endif
