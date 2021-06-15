#include "Scene.h"

#include "Debug.h"

#include <GL/glut.h>

Scene::Scene()
    : bPolygonFill(true), bPolygonBB(false), Occlusion(), ManagerLOD() {}

Scene::~Scene() {
  for (auto &m : mMeshes) {
    delete m;
  }
  mMeshes.clear();
}

void Scene::init(const std::string &fn, CullingMethod cm) {
  this->Occlusion::init(cm);
  filename = fn;
  std::string formatFile =
      std::string(&filename[filename.size() - 3], &filename[filename.size()]);
  if (formatFile == "ply") {
    loadMesh(filename);
  } else if (formatFile == "txt") {
    loadTileMap(filename);
  }
}

void Scene::update(int deltaTime) {
  mPlayer.update(deltaTime);
  this->ManagerLOD::update(deltaTime, mMeshes, mPlayer);
  this->Occlusion::update(deltaTime);
}

void Scene::render() {
  if (mMeshes.size() > 0) {
    testFrustrumCulling();
    mBasicProgram.use();
    mBasicProgram.setUniformMatrix4f("projection",
                                     mPlayer.getProjectionMatrix());
    mBasicProgram.setUniformMatrix4f("view", mPlayer.getViewMatrix());
    mBasicProgram.setUniform1i("bLighting", bPolygonFill ? 1 : 0);
    mBasicProgram.setUniform4f("color", 0.9f, 0.9f, 0.95f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (mQttyMean == QTTY_MEAN) {
      mQttyMean = 1;
      mQttyTriangles = 0;
    } else {
      ++mQttyMean;
    }
    for (auto &mesh : mMeshes) {
      mBasicProgram.setUniformMatrix4f("model", mesh->getModelMatrix());
      if (!bPolygonFill) {
        mBasicProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(0.5f, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        mesh->render();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_POLYGON_OFFSET_FILL);
        mBasicProgram.setUniform4f("color", 0.0f, 0.0f, 0.0f, 1.0f);
      }
      if (!bPolygonBB) {
        mesh->render();
      } else {
        mesh->renderBoundinBox();
      }
      mQttyTriangles += mesh->getTriangleSize();
    }
    this->Occlusion::render();
    testOcclusion();
  }
}

void Scene::keyEvent(int key, int specialkey, bool pressed) {
  if (pressed) {
    std::string formatFile =
        std::string(&filename[filename.size() - 3], &filename[filename.size()]);
    if (formatFile == "ply") {
      if (key == '+')
        loadMesh(filename);
      else if (key == '-')
        unloadMesh(filename);
      else if ((key >= '0') && (key <= '9')) {
        int num = int(key) - int('0');
        while (mMeshes.size() != num) {
          if (mMeshes.size() > num) {
            unloadMesh(filename);
          } else {
            loadMesh(filename);
          }
        }
      }
    }
    if (specialkey == GLUT_KEY_F1) {
      bPolygonFill = !bPolygonFill;
    }
    if (specialkey == GLUT_KEY_F2) {
      bPolygonBB = !bPolygonBB;
    }
  }
  this->Occlusion::keyEvent(key, specialkey, pressed);
  mPlayer.keyEvent(key, specialkey, pressed);
}

void Scene::mouseMove(int x, int y, const glm::ivec2 &lastMousePos,
                      bool *mouseButtons) {
  mPlayer.mouseMove(x, y, lastMousePos, mouseButtons);
}

void Scene::resize(int width, int height) { mPlayer.resize(width, height); }
