#include "KdTree.h"

#include "Debug.h"
namespace Axis {
inline static const int DIM = 3;
inline static const glm::vec3 Z = {0.0f, 0.0f, 1.0f};
inline static const glm::vec3 X = {1.0f, 0.0f, 0.0f};
inline static const glm::vec3 Y = {0.0f, 1.0f, 0.0f};
static int axis(int i) { return i % DIM; }
static const glm::vec3 next(int i) {
  switch (axis(i)) {
  default:
  case 0:
    return X;
  case 1:
    return Y;
  case 2:
    return Z;
  }
}
}; // namespace Axis

KdTree::KdTree(int level)
    : mLevel(level), mPoss(0.0f), mFather(nullptr), mElementsAABB(),
      mGlobalAABB() {}

KdTree::KdTree(const std::list<Mesh *> &elements, int level) : KdTree(level) {
  int size = elements.size();
  if (size > 1) {
    glm::vec3 poss(0.0f);
    for (const auto &e : elements) {
      if (e->addToKdTree()) {
        poss += e->getPos();
      }
    }
    mPoss = poss / float(size);
    bool correct = false;
    for (unsigned int i = 0; i < Axis::DIM; ++i) {
      std::list<Mesh *> positive, mid, negative;
      mAxisID = Axis::axis(mLevel + i);
      float point = mPoss[mAxisID];
      for (auto &e : elements) {
        if (e->addToKdTree()) {
          float mesh_size = e->getSize()[mAxisID];
          float lower = e->getPos()[mAxisID];
          float upper = lower + mesh_size;
          if (point > lower && point < upper) {
            mid.push_back(e);
          } else if (point <= lower) {
            negative.push_back(e);
          } else if (point >= upper) {
            positive.push_back(e);
          } else {
            cerr << "ERROR: point vs line" << std::endl;
          }
        }
      }

      if (negative.size() > 0 && positive.size() > 0) {
        correct = true;
        KdTree two = KdTree(positive, level + 1);
        two.mFather = this;
        KdTree one = KdTree(negative, level + 1);
        one.mFather = this;
        mChildrens.push_back(one);
        mChildrens.push_back(two);
        for (const auto &e : mid) {
          mElements.push_back(e);
          mElementsAABB.buildCube(e->getPos(), e->getSize());
        }
        mGlobalAABB = Mesh(mElementsAABB.getMin());
        mGlobalAABB.buildCube({0, 0, 0}, mElementsAABB.getSize());
      }
    }
    if (!correct) {
      for (const auto &e : elements) {
        mElements.push_back(e);
      }
    }
  } else if (size == 1) {
    mElements.push_back(*elements.begin());
  }
}

void KdTree::pullUpVisibility() {
  if (!mVisible) {
    mVisible = true;
    for (auto &e : mElements) {
      e->setOcclusion(false);
    }
    if (mFather != nullptr) {
      mFather->pullUpVisibility();
    }
  }
}
void KdTree::traverseNode(std::stack<KdTree *> &traversalStack) {
  // because we have "middle" objects we render always
  renderModels();
  for (auto &c : mChildrens) {
    traversalStack.push(&c);
  }
}

Visibility KdTree::computeVisibility() {
  Visibility result;
  result.wasVisible = mVisible && (mLastFrameVisible == OCCLUDED_FRAMES - 1);
  result.opened = result.wasVisible && !isLeaf();
  mVisible = false;
  return result;
}

void KdTree::renderModels() const {
  for (auto &e : mElements) {
    e->render();
  }
}

void KdTree::render() const {
  for (auto &c : mChildrens) {
    c.render();
  }
  mElementsAABB.render();
  mGlobalAABB.render();
}

void KdTree::nextFrame() {
  --mLastFrameVisible;
  for (auto &c : mChildrens) {
    c.nextFrame();
  }
}

const Mesh &KdTree::getAABBMesh() const { return mGlobalAABB; }

KdTree KdTree::getChildren(int i) const { return mChildrens[i]; }
int KdTree::getQttyChildrens() const { return mChildrens.size(); }
const Mesh *KdTree::getElement(int i) const { return mElements[i]; }
int KdTree::getQttyElements() const { return mElements.size(); }
bool KdTree::isLeaf() const { return mChildrens.size() == 0; }
int KdTree::getLevel() const { return mLevel; }
glm::vec3 KdTree::getAxis() const { return Axis::next(mAxisID); }
