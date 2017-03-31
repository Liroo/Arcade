#include "Core.hpp"

// __attributes__ is to remove to display help of soft
int main(int argc [[gnu::unused]], char *argv[])
{
    Arcade::Core software;

    if (software.init(argv[1])) {
      software.run();
    }

    return 0;
}
