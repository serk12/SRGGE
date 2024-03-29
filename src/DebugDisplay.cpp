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

void DebugDisplay::setQttyTriangles(int triangles) {
  qttyTriangles = triangles;
}

bool DebugDisplay::render() {
  float avg_dt = deltaTime / qtty;
  int fps = 1.0f / avg_dt * 1000.0f;
  int tps = qttyTriangles / avg_dt;
  int maxCost = tps / fps;
  std::string s = std::to_string(fps) + " (fps) / " + std::to_string(tps) +
                  " (tps) / " + std::to_string(maxCost) + " (MC) ";
  textRender.renderText(s, 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 1.0f, 0.2f));
  return true;
}
