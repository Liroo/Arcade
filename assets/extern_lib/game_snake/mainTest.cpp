# include "Snake.h"

int main() {
  Arcade::AGames* snake = load_lib();

  snake->start();
  for (int i = 0; i < 20; i++) {
    snake->tick();
  }
  return 0;
}
