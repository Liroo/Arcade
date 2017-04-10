#include "LibGameTest.h"

LibGameTest::LibGameTest() {}
LibGameTest::~LibGameTest() {}

void LibGameTest::start() {

}

void LibGameTest::reset() {

}

Arcade::ObjectList LibGameTest::tick() {
  return _objectListTest;
}

Arcade::ObjectList LibGameTest::handleEvent(const Arcade::Event&) {
  return _objectListTest;
}

Arcade::ObjectList LibGameTest::dump() const {
  return _objectListTest;
}

std::map<std::string, std::string> LibGameTest::dumpMemory() const {
  return _memoryTest;
}
