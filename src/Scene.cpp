#include "Debug.h"
#include <cmath>
#define GLM_FORCE_RADIANS
#include "PLYReader.h"
#include "Scene.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TileMapLoader.h"
#include <GL/glut.h>

Scene::Scene() {}

Scene::~Scene() {
  for (auto &m : meshes) {
    delete m;
  }
  meshes.clear();
}

void Scene::init(const std::string &fn) {
  bPolygonFill = true;
  filename = fn;
  basicProgram.initShaders("shaders/basic.vert", "shaders/basic.frag");
  player.init();
  std::string formatFile =
      std::string(&filename[filename.size() - 3], &filename[filename.size()]);
  if (formatFile == "ply") {
    loadMesh();
  } else if (formatFile == "txt") {
    loadTileMap();
  }
}

void Scene::loadMesh() {
  std::string formatFile =
      std::string(&filename[filename.size() - 3], &filename[filename.size()]);
  if (formatFile == "ply") {
    loadMesh(filename, next_pos);
    next_pos.x = (int(next_pos.x) + 1) % 2;
    next_pos.z = (int(next_pos.z) + int(next_pos.x == 0));
  } else {
    // this should not happen
    std::cerr << "filename is a txt" << std::endl;
  }
}

void Scene::loadMesh(const std::string &fn, glm::vec3 pos) {
  TriangleMesh *mesh = new TriangleMesh(pos);
  meshes.push_front(mesh);
  PLYReader reader;
  bool bSuccess = reader.readMesh(fn, *mesh);
  if (bSuccess)
    mesh->sendToOpenGL(basicProgram);
}

void Scene::loadTileMap() {
  TileMapModels tilemap = TileMapLoader::instance().load(filename);
  int i = -tilemap.size()/2;
  for (auto t : tilemap) {
    int j = -tilemap[0].size()/2;
    for (std::string m : t) {
      if (m != TileMapLoader::EMPTY) {
        if (m != TileMapLoader::WALL) {
          loadMesh(TileMapLoader::GROUND, glm::vec3(i, -1.5f, j));
        }
        if (m != TileMapLoader::GROUND) {
          loadMesh(m, glm::vec3(i, -1.0f, j));
        }
      }
      ++j;
    }
    ++i;
  }
}

void Scene::unloadMesh() {
  if (meshes.size() > 0) {
    TriangleMesh *mesh = meshes.back();
    meshes.pop_back();

    next_pos.x = (int(next_pos.x) - 1) % 2;
    next_pos.z = (int(next_pos.z) - 1);
  }
}

void Scene::update(int deltaTime) { player.update(deltaTime); }

void Scene::render() {
  if (meshes.size() > 0) {
    basicProgram.use();
    basicProgram.setUniformMatrix4f("projection", player.getProjectionMatrix());
    basicProgram.setUniformMatrix4f("view", player.getViewMatrix());
    basicProgram.setUniform1i("bLighting", bPolygonFill ? 1 : 0);
    basicProgram.setUniform4f("color", 0.9f, 0.9f, 0.95f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (auto &mesh : meshes) {
      basicProgram.setUniformMatrix4f("model", mesh->getModelMatrix());
      if (!bPolygonFill) {
        basicProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(0.5f, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        mesh->render();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_POLYGON_OFFSET_FILL);
        basicProgram.setUniform4f("color", 0.0f, 0.0f, 0.0f, 1.0f);
      }
      mesh->render();
    }
  }
}

void Scene::keyEvent(int key, int specialkey, bool pressed) {
  if (pressed) {
    std::string formatFile =
        std::string(&filename[filename.size() - 3], &filename[filename.size()]);
    if (formatFile == "ply") {
      if (key == '+')
        loadMesh();
      else if (key == '-')
        unloadMesh();
      else if ((key >= '0') && (key <= '9')) {
        int num = int(key) - int('0');
        while (meshes.size() != num) {
          if (meshes.size() > num) {
            unloadMesh();
          } else {
            loadMesh();
          }
        }
      }
    }

    if (specialkey == GLUT_KEY_F1) {
      bPolygonFill = !bPolygonFill;
    }
  }
  player.keyEvent(key, specialkey, pressed);
}

void Scene::mouseMove(int x, int y, const glm::ivec2 &lastMousePos,
                      bool *mouseButtons) {
  player.mouseMove(x, y, lastMousePos, mouseButtons);
}

void Scene::resize(int width, int height) { player.resize(width, height); }
