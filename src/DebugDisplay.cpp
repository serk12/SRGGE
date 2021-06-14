#include "DebugDisplay.h"

#include "Debug.h"
#include "Define.h"

DebugDisplay::DebugDisplay() {}

void DebugDisplay::init() {
  textRender.init();
  deltaTime = -1.0f;
  qtty = 1;
  accDeltaTime = 0;
}

bool DebugDisplay::update(int dt) {
  deltaTime = dt;
  if (qtty > 10) {
    accDeltaTime = dt;
    qtty = 1;
  } else {
    accDeltaTime += dt;
    ++qtty;
  }

  return true;
}

void DebugDisplay::setQttyTriangles(int triangles) {
  qttyTriangles = triangles;
}

bool DebugDisplay::render() {
  float avg_dt = accDeltaTime / qtty;
  if (avg_dt <= 0.001f) {
    return false;
  }
  int fps = 1.0f / avg_dt * 1000.0f;
  int tps = qttyTriangles / avg_dt * 1000.0f;
  int maxCost = tps * FPS;
  std::string s = std::to_string(fps) + " (fps) / " +
                  std::to_string(qttyTriangles) + " (#tr) / " +
                  std::to_string(maxCost) + " (MC) ";
  textRender.renderText(s, 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 1.0f, 0.2f));
  return true;
}
