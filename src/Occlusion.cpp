#include "Occlusion.h"

#include "Debug.h"

#include <GL/glut.h>
#include <fstream>
#include <queue>
#include <sstream>
#include <stack>

const int Occlusion::VISIBLE_PIXELS_THRESHOLD = 200;

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

void Occlusion::setOcclusionMap(const std::string &m) {
  OCCLUSION_MAP = m;
  int header = 0;
  std::ifstream map(OCCLUSION_MAP);
  for (std::string line; std::getline(map, line);) {
    std::istringstream buf(line);
    std::vector<bool> row;
    if (header > 1) {
      for (bool element; buf >> element;) {
        row.push_back(element);
      }
      msOcclusionMap.push_back(row);
    } else {
      for (int element; buf >> element;) {
        if (header == 0) {
          msX = element;
        } else {
          msY = element;
        }
        ++header;
      }
    }
  }
}

Occlusion::Occlusion(CullingMethod cm)
    : mCullingPolicy(cm), mBKDTree(-1), mFrame(0), mKDTree(nullptr) {}

void Occlusion::init(CullingMethod cm) {
  this->BasicScene::init();
  mKDTree = new KdTree(mMeshes);
  mCullingPolicy = cm;
}

void Occlusion::render() const {
  if (mKDTree != nullptr) {
    if (mBKDTree >= 0 && mBKDTree <= KdTree::MAX_DEEP + 1) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      mBasicProgram.setUniform4f("color", 0.0f, 0.0f, 0.0f, 1.0f);
      mKDTree->render(mBasicProgram, mBKDTree);
    }
  }
}
void Occlusion::update(int deltaTime) {
  ++mFrame;
  if (mFrame == std::numeric_limits<unsigned int>::max()) {
    mFrame = 0;
  }
}

bool Occlusion::viewCulling(const Mesh &mesh) {
  auto frustum = mPlayer.getFrustum();
  for (auto &p : frustum) {
    if (mesh.planeTest(p) != Collision::Positive) {
      return false;
    }
  }
  return true;
}

// https://www.mbsoftworks.sk/tutorials/opengl3/27-occlusion-query/
void Occlusion::occlusionCullingSaW() {
  GLuint queries[mMeshes.size()];
  glGenQueries(mMeshes.size(), queries);
  unsigned int i = 0;
  for (auto &mesh : mMeshes) {
    if (mesh->canAddToKdTree() && mesh->stillVisible() &&
        (mCullingPolicy == OCCLUSION || mesh->isInsideFrustum())) {
      mBasicProgram.setUniformMatrix4f("model", mesh->getModelMatrix());
      glBeginQuery(GL_SAMPLES_PASSED, queries[i]);
      mesh->renderBoundinBox();
      glEndQuery(GL_SAMPLES_PASSED);
    }
    ++i;
  }
  i = 0;
  for (auto &mesh : mMeshes) {
    if (mesh->canAddToKdTree() && mesh->stillVisible() &&
        (mCullingPolicy == OCCLUSION || mesh->isInsideFrustum())) {
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

void Occlusion::occlusionCulling() {
  std::stack<KdTree *> traversalStack;
  std::queue<Query *> queryQueue;

  traversalStack.push(mKDTree);
  while (!traversalStack.empty() || !queryQueue.empty()) {
    while (!queryQueue.empty() &&
           (queryQueue.front()->getQuery() || traversalStack.empty())) {
      queryQueue.front()->waitQuery();
      auto query = queryQueue.front();
      queryQueue.pop();
      if (query->qttyVisiblePixels == -1) {
        Debug::error("kdtree Occlusion query ERROR");
      } else if (query->qttyVisiblePixels >
                 VISIBLE_PIXELS_THRESHOLD * query->tree->getQttyElements()) {
        query->tree->pullUpVisibility();
        query->tree->traverseNode(traversalStack, mBasicProgram);
      }
      delete query;
    }
    if (!traversalStack.empty()) {
      auto next_tree = traversalStack.top();
      traversalStack.pop();
      if (next_tree->getQttyElements() == 0) {
        next_tree->traverseNode(traversalStack, mBasicProgram);
      }
      if (viewCulling(next_tree->getAABBMesh())) {
        auto t = *next_tree;
        Visibility visibility = next_tree->computeVisibility(mFrame);
        if (!visibility.opened) {
          auto *newQuery = new Query(next_tree);
          glBeginQuery(GL_SAMPLES_PASSED, newQuery->queryID);
          newQuery->tree->renderModels(mBasicProgram);
          glEndQuery(GL_SAMPLES_PASSED);
          queryQueue.push(newQuery);
        }
        if (visibility.wasVisible) {
          next_tree->traverseNode(traversalStack, mBasicProgram);
        }
      }
    }
  }
  mKDTree->nextFrame();
}

void Occlusion::keyEvent(int key, int specialkey, bool pressed) {
  if (specialkey == GLUT_KEY_F4) {
    mBKDTree = (mBKDTree + 1) % (KdTree::MAX_DEEP + 3);
  }
  if (specialkey == GLUT_KEY_F3) {
    if (mBKDTree <= KdTree::MAX_DEEP + 1) {
      mBKDTree = KdTree::MAX_DEEP + 2;
    } else {
      mBKDTree = KdTree::MAX_DEEP + 1;
    }
  }
}

void Occlusion::testFrustrumCulling() {
  bool view = mCullingPolicy == ALL || mCullingPolicy == VIEW ||
              mCullingPolicy == ALL_SAW;

  for (auto &mesh : mMeshes) {
    if (view) {
      mesh->setInsideFrustum(viewCulling(*mesh));
    }
  }
}

void Occlusion::testOcclusion() {
  bool occluded_kd = mCullingPolicy == ALL || mCullingPolicy == OCCLUSION;
  bool occluded_saw =
      mCullingPolicy == ALL_SAW || mCullingPolicy == OCCLUSION_SAW;
  bool occluded = (occluded_kd || occluded_saw) && mKDTree != nullptr;
  if (occluded) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    if (occluded_saw) {
      occlusionCullingSaW();
    }
    if (occluded_kd) {
      occlusionCulling();
    }
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
  } else if (mCullingPolicy == CULLING_MAP && OCCLUSION_MAP != "") {
    auto pos = mPlayer.getPos();
    pos.x += msX / 2.0f;
    pos.z += msY / 2.0f;
    int i;
    if (msY < msX) {
      i = int(pos.x) + int(pos.z) * msX;
    } else {
      i = int(pos.x) * msY + int(pos.z);
    }
    // outside occluding map
    if (pos.x < 0 || pos.y < 0 || i < 0 || i >= msOcclusionMap.size()) {
      for (auto &m : mMeshes) {
        m->setOcclusion(true);
      }
    } else {
      int j = 0;
      for (auto &m : mMeshes) {
        m->setOcclusion(!msOcclusionMap[i][j]);
        ++j;
      }
    }
  }
}
