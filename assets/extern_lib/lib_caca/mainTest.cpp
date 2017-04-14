# include "LibCaca.h"

int main() {
  Arcade::IGraphic* caca = load_lib();

  caca->init([](const Arcade::Event&) -> Arcade::Object {
    return {};
  });
  caca->run();
  return 0;
}
