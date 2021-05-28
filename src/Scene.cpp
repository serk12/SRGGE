#include "Debug.h"
#include <cmath>
#include <queue>
#include <stack>
#define GLM_FORCE_RADIANS
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

void Scene::init(const std::string &fn, CullingMethod cm) {
  cullingPolicy = cm;
  bPolygonFill = true;
  filename = fn;
  basicProgram.initShaders("shaders/basic.vert", "shaders/basic.frag");
  Mesh::setShaderProgram(&basicProgram);
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
    Mesh *mesh = new Mesh(filename, next_pos);
    meshes.push_back(mesh);
    next_pos.x = (int(next_pos.x) + 1) % 2;
    next_pos.z = (int(next_pos.z) + int(next_pos.x == 0));
  } else {
    // this should not happen
    std::cerr << "filename is a txt" << std::endl;
  }
}

void Scene::loadTileMap() {
  TileMapModels tilemap = TileMapLoader::instance().load(filename);
  int i = -tilemap.size() / 2;
  for (auto t : tilemap) {
    int j = -tilemap[0].size() / 2;
    for (std::string m : t) {
      if (m != TileMapLoader::EMPTY) {
        Mesh *mesh = new Mesh(m, glm::vec3(i, -1.0f, j));
        meshes.push_back(mesh);
      }
      ++j;
    }
    ++i;
  }
  kdTree = KdTree(meshes);
  Debug::print(kdTree);
}

void Scene::unloadMesh() {
  if (meshes.size() > 0) {
    Mesh *mesh = meshes.back();
    delete mesh;
    meshes.pop_back();

    next_pos.x = (int(next_pos.x) - 1) % 2;
    next_pos.z = (int(next_pos.z) - 1);
  }
}

void Scene::update(int deltaTime) { player.update(deltaTime); }

bool Scene::viewCulling(const Mesh &mesh) {
  auto frustum = player.getFrustum();
  for (auto &p : frustum) {
    if (mesh.planeTest(p) != Collision::Positive) {
      return false;
    }
  }
  return true;
}
void Scene::occlusionCulling() {
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);
  std::stack<KdTree *> traversalStack;
  std::queue<Query *> queryQueue;
  int qttyQueries = 0;
  traversalStack.push(&kdTree);
  while (!traversalStack.empty() || !queryQueue.empty()) {
    while (!queryQueue.empty() &&
           (queryQueue.front()->getQuery() || traversalStack.empty())) {
      auto query = queryQueue.front();
      queryQueue.pop();
      if (query->qttyVisiblePixels > VISIBLE_PIXELS_THRESHOLD) {
        query->tree->pullUpVisibility();
        query->tree->traverseNode(traversalStack);
      }
      delete query;
    }
    if (!traversalStack.empty()) {
      auto tree = traversalStack.top();
      traversalStack.pop();
      if (viewCulling(tree->getAABBMesh())) {
        Visibility visibility = tree->computeVisibility();
        if (visibility.wasVisible) {
          tree->traverseNode(traversalStack);
        }
        if (!visibility.opened) {
          auto *newQuery = new Query(tree, qttyQueries);
          // https://www.mbsoftworks.sk/tutorials/opengl3/27-occlusion-query/
          glBeginQuery(GL_SAMPLES_PASSED, qttyQueries);
          tree->renderModels();
          glEndQuery(GL_SAMPLES_PASSED);
          ++qttyQueries;
          queryQueue.push(newQuery);
        }
      }
    }
  }
}

void Scene::render() {
  if (meshes.size() > 0) {
    bool view = cullingPolicy == ALL || cullingPolicy == VIEW;
    bool occluded = cullingPolicy == ALL || cullingPolicy == OCCLUSION;
    for (auto &mesh : meshes) {
      mesh->resetOcclusion(view, occluded);
      if (view) {
        mesh->setInsideFrustum(viewCulling(*mesh));
      }
    }
    if (occluded) {
      occlusionCulling();
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      glDepthMask(GL_TRUE);
    }

    basicProgram.use();
    basicProgram.setUniformMatrix4f("projection", player.getProjectionMatrix());
    basicProgram.setUniformMatrix4f("view", player.getViewMatrix());
    basicProgram.setUniform1i("bLighting", bPolygonFill ? 1 : 0);
    basicProgram.setUniform4f("color", 0.9f, 0.9f, 0.95f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (auto &mesh : meshes) {
      if (mesh->isVisible()) {
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
    if (!bPolygonFill) {
      glm::mat4 identiti = glm::mat4(1.0f);
      basicProgram.setUniformMatrix4f("model", identiti);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDisable(GL_POLYGON_OFFSET_FILL);
      basicProgram.setUniform4f("color", 0.0f, 0.0f, 0.0f, 1.0f);
      kdTree.render();
    }
    kdTree.nextFrame();
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
