#ifndef BASIC_SCENE_INCLUDE
#define BASIC_SCENE_INCLUDE

#include "Mesh.h"
#include "Player.h"
#include "ShaderProgram.h"

#include <list>

class BasicScene {

public:
  void resize(int width, int height);

protected:
  void init();
  void loadMesh(const std::string &filename);
  void loadTileMap(const std::string &filename);
  void unloadMesh(const std::string &filename);

protected:
  std::list<Mesh *> mMeshes;
  mutable ShaderProgram mBasicProgram;
  glm::vec3 mNextPos = {0, -1, 0};
  Player mPlayer;
};

#endif
