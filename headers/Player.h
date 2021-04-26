#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Camera.h"

class Player : public Camera {
public:
  void init();
  void update(int deltaTime);
  void resize(int width, int height);
  void mouseMove(int x, int y, const glm::ivec2 &lastMousePos,
                 bool *mouseButtons);
  void keyEvent(int key, int specialkey, bool pressed);

private:
  glm::vec3 vel;
};

#endif // ifndef _PLAYER_INCLUDE
