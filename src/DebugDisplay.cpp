#include "DebugDisplay.h"

DebugDisplay::DebugDisplay() {}

void DebugDisplay::init() {
  textRender.init();
  deltaTime = -1.0f;
  qtty = 1;
}

bool DebugDisplay::update(int dt) {
  if (qtty > 10) {
    deltaTime = dt;
    qtty = 1;
  } else {
    deltaTime += dt;
    ++qtty;
  }

  return true;
}

bool DebugDisplay::render() {
  int fps = 1.0f / (deltaTime / qtty) * 1000.0f;
  std::string s = std::to_string(fps);
  textRender.renderText(s, 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 1.0f, 0.2f));
  return true;
}
