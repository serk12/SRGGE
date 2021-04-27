#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include "Player.h"
#include "ShaderProgram.h"
#include "TriangleMesh.h"
#include <glm/glm.hpp>
#include <list>

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene {
public:
  Scene();
  ~Scene();

  void init(const std::string &fn = "");
  void render();
  void update(int deltaTime);

  void resize(int width, int height);
  void keyEvent(int key, int specialkey, bool pressed);
  void mouseMove(int x, int y, const glm::ivec2 &lastMousePos,
                 bool *mouseButtons);

private:
  void loadMesh();
  void loadMesh(const std::string &fn, glm::vec3 pos = {0, -1, 0});
  void unloadMesh();
  void initShaders();
  void loadTileMap();

private:
  std::list<TriangleMesh *> meshes;
  ShaderProgram basicProgram;
  Player player;
  bool bPolygonFill;
  std::string filename; // path to last model loaded
  glm::vec3 next_pos = {0, -1, 0};
};

#endif // _SCENE_INCLUDE