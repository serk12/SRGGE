#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include "ManagerLOD.h"
#include "Occlusion.h"

#include <glm/glm.hpp>
#include <list>

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene : public Occlusion, public ManagerLOD {
public:
  Scene();
  ~Scene();

  void init(const std::string &fn = "", CullingMethod cm = NONE);
  void render();
  void update(int deltaTime);

  void resize(int width, int height);
  void keyEvent(int key, int specialkey, bool pressed);
  void mouseMove(int x, int y, const glm::ivec2 &lastMousePos,
                 bool *mouseButtons);

private:
  bool bPolygonFill, bPolygonBB;
  std::string filename; // path to last model loaded
};

#endif // _SCENE_INCLUDE
