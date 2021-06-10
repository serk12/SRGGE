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

const int Scene::VISIBLE_PIXELS_THRESHOLD = 3000;

Query::Query(KdTree *t) {
  GLuint queries[1];
  glGenQueries(1, queries);
  tree = t;
  queryID = queries[0];
  qttyVisiblePixels = -1;
  done = false;
}

bool Query::getQuery() {
  GLint i_done;
  glGetQueryObjectiv(queryID, GL_QUERY_RESULT_AVAILABLE, &i_done);
  done = i_done == GL_TRUE;
  if (done) {
    glGetQueryObjectiv(queryID, GL_QUERY_RESULT, &qttyVisiblePixels);
  }
  return done;
}

void Query::waitQuery() {
  if (done)
    return;
  while (!done) {
    GLint i_done;
    glGetQueryObjectiv(queryID, GL_QUERY_RESULT_AVAILABLE, &i_done);
    done = i_done == GL_TRUE;
  }
  glGetQueryObjectiv(queryID, GL_QUERY_RESULT, &qttyVisiblePixels);
}

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
  bPolygonBB = false;
  filename = fn;
  bKDTree = -1;
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
    Debug::error("filename is a txt");
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
  kdTree = new KdTree(meshes);
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

// https://www.mbsoftworks.sk/tutorials/opengl3/27-occlusion-query/
void Scene::occlusionCullingSaW() {
  GLuint queries[meshes.size()];
  glGenQueries(meshes.size(), queries);
  unsigned int i = 0;
  for (auto &mesh : meshes) {
    if (mesh->canAddToKdTree() &&
        (cullingPolicy == OCCLUSION || mesh->isInsideFrustum())) {
      basicProgram.setUniformMatrix4f("model", mesh->getModelMatrix());
      glBeginQuery(GL_SAMPLES_PASSED, queries[i]);
      mesh->renderBoundinBox();
      glEndQuery(GL_SAMPLES_PASSED);
    }
    ++i;
  }
  i = 0;
  for (auto &mesh : meshes) {
    if (mesh->canAddToKdTree() &&
        (cullingPolicy == OCCLUSION || mesh->isInsideFrustum())) {
      int done = 0;
      while (!done) {
        glGetQueryObjectiv(queries[i], GL_QUERY_RESULT_AVAILABLE, &done);
      }
      int result = -1;
      glGetQueryObjectiv(queries[i], GL_QUERY_RESULT, &result);
      if (result == -1) {
        Debug::error("S&W Occlusion query ERROR");
      }
      mesh->setOcclusion(result < VISIBLE_PIXELS_THRESHOLD);
    }
    ++i;
  }
}

void Scene::occlusionCulling() {
  std::stack<KdTree *> traversalStack;
  std::queue<Query *> queryQueue;

  traversalStack.push(kdTree);
  while (!traversalStack.empty() || !queryQueue.empty()) {
    while (!queryQueue.empty() &&
           (queryQueue.front()->getQuery() || traversalStack.empty())) {
      queryQueue.front()->waitQuery();
      auto query = queryQueue.front();
      queryQueue.pop();
      if (query->qttyVisiblePixels == -1) {
        Debug::error("kdtree Occlusion query ERROR");
      } else if (query->qttyVisiblePixels < VISIBLE_PIXELS_THRESHOLD) {
        query->tree->pullUpVisibility();
        query->tree->traverseNode(traversalStack, basicProgram);
      }
      delete query;
    }
    if (!traversalStack.empty()) {
      auto next_tree = traversalStack.top();
      traversalStack.pop();
      if (cullingPolicy == OCCLUSION || viewCulling(next_tree->getAABBMesh())) {
        auto t = *next_tree;
        Visibility visibility = next_tree->computeVisibility();
        if (visibility.wasVisible) {
          next_tree->traverseNode(traversalStack, basicProgram);
        }
        if (!visibility.opened) {
          auto *newQuery = new Query(next_tree);
          glBeginQuery(GL_SAMPLES_PASSED, newQuery->queryID);
          newQuery->tree->renderModels(basicProgram);
          glEndQuery(GL_SAMPLES_PASSED);
          queryQueue.push(newQuery);
        }
      }
    }
  }
  kdTree->nextFrame();
}

void Scene::render() {
  if (meshes.size() > 0) {
    bool view = cullingPolicy == ALL || cullingPolicy == VIEW;
    bool occluded = cullingPolicy == ALL || cullingPolicy == OCCLUSION;
    for (auto &mesh : meshes) {
      if (view) {
        mesh->setInsideFrustum(viewCulling(*mesh));
      }
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
        if (!bPolygonBB) {
          mesh->render();
        } else {
          mesh->renderBoundinBox();
        }
      }
    }

    if (occluded) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glDepthMask(GL_FALSE);
      occlusionCullingSaW();
      // occlusionCulling();
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      glDepthMask(GL_TRUE);
    }
    if (kdTree != nullptr) {
      if (bKDTree >= 0 && bKDTree <= KdTree::MAX_DEEP + 1) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_POLYGON_OFFSET_FILL);
        basicProgram.setUniform4f("color", 0.0f, 0.0f, 0.0f, 1.0f);
        kdTree->render(basicProgram, bKDTree);
      }
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
    if (specialkey == GLUT_KEY_F2) {
      bPolygonBB = !bPolygonBB;
    }
    if (specialkey == GLUT_KEY_F4) {
      bKDTree = (bKDTree + 1) % (KdTree::MAX_DEEP + 3);
    }
    if (specialkey == GLUT_KEY_F3) {
      if (bKDTree <= KdTree::MAX_DEEP + 1) {
        bKDTree = KdTree::MAX_DEEP + 2;
      } else {
        bKDTree = KdTree::MAX_DEEP + 1;
      }
    }
  }
  player.keyEvent(key, specialkey, pressed);
}

void Scene::mouseMove(int x, int y, const glm::ivec2 &lastMousePos,
                      bool *mouseButtons) {
  player.mouseMove(x, y, lastMousePos, mouseButtons);
}

void Scene::resize(int width, int height) { player.resize(width, height); }
