#include "Debug.h"
#include "Player.h"

void Player::mouseMove(int x, int y, const glm::ivec2 &lastMousePos,
                       bool *mouseButtons) {
  // Rotation
  if (mouseButtons[0] && (lastMousePos.x != -1))
    rotateCamera(0.5f * (y - lastMousePos.y), 0.5f * (x - lastMousePos.x));
}

void Player::resize(int width, int height) {
  resizeCameraViewport(width, height);
}

void Player::init() { Camera::init(); }

void Player::update(int deltaTime) { displaceCamera(vel * float(deltaTime)); }

void Player::keyEvent(int key, int specialkey, bool pressed) {
  if (pressed) {
    if (key == 'w') {
      vel.z = 0.001f;
    } else if (key == 's') {
      vel.z = -0.001f;
    } else if (key == 'd') {
      vel.x = -0.001f;
    } else if (key == 'a') {
      vel.x = 0.001f;
    } else if (key == ' ') {
      vel.y = -0.001f;
    } else if (key == 'c') {
      vel.y = 0.001f;
    }
  } else {
    if (key == 'w') {
      vel.z = 0.0f;
    } else if (key == 's') {
      vel.z = 0.0f;
    } else if (key == 'd') {
      vel.x = 0.0f;
    } else if (key == 'a') {
      vel.x = 0.0f;
    } else if (key == ' ') {
      vel.y = 0.0f;
    } else if (key == 'c') {
      vel.y = 0.0f;
    }
  }
}
