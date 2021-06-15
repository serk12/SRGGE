#include "BasicScene.h"

#include "Debug.h"
#include "TileMapLoader.h"

#include <string>

void BasicScene::init() {
  mBasicProgram.initShaders("shaders/basic.vert", "shaders/basic.frag");
  Mesh::setShaderProgram(&mBasicProgram);
  mPlayer.init();
}

void BasicScene::loadMesh(const std::string &filename) {
  std::string formatFile =
      std::string(&filename[filename.size() - 3], &filename[filename.size()]);
  if (formatFile == "ply") {
    Mesh *mesh = new Mesh(filename, mNextPos);
    mMeshes.push_back(mesh);
    mNextPos.x = (int(mNextPos.x) + 1) % 2;
    mNextPos.z = (int(mNextPos.z) + int(mNextPos.x == 0));
  } else {
    // this should not happen
    Debug::error("filename is a txt");
  }
}

void BasicScene::unloadMesh(const std::string &filename) {
  if (mMeshes.size() > 0) {
    Mesh *mesh = mMeshes.back();
    delete mesh;
    mMeshes.pop_back();

    mNextPos.x = (int(mNextPos.x) - 1) % 2;
    mNextPos.z = (int(mNextPos.z) - 1);
  }
}

void BasicScene::loadTileMap(const std::string &filename) {
  TileMapModels tilemap = TileMapLoader::instance().load(filename);
  int i = -tilemap.size() / 2;
  for (auto t : tilemap) {
    int j = -tilemap[0].size() / 2;
    for (std::string m : t) {
      if (m != TileMapLoader::EMPTY) {
        Mesh *mesh = new Mesh(m, glm::vec3(i, -1.0f, j));
        mMeshes.push_back(mesh);
      }
      ++j;
    }
    ++i;
  }
}
