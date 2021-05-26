#include "KdTree.h"

#include "Debug.h"

KdTree::KdTree(int level) : mLevel(level), mPoss(0.0f) {}

KdTree::KdTree(const std::list<Mesh *> &elements, int level) : KdTree(level) {
  int size = elements.size();
  if (size > 1) {
    glm::vec3 poss(0.0f);
    for (const auto &e : elements) {
      poss += e->getPos();
    }
    mPoss = poss / float(size);
    bool correct = false;
    for (unsigned int i = 0; i < Axis::DIM; ++i) {
      std::list<Mesh *> positive, mid, negative;
      mAxisID = Axis::axis(mLevel + i);
      float point = mPoss[mAxisID];
      for (auto e : elements) {
        float mesh_size = e->getSize()[mAxisID];
        float lower = e->getPos()[mAxisID];
        float upper = lower + mesh_size;
        if (point >= lower && point < upper) {
          mid.push_back(e);
        } else if (point < lower) {
          negative.push_back(e);
        } else if (point >= upper) {
          positive.push_back(e);
        } else {
          cerr << "ERROR:" << std::endl;
        }
      }

      if (negative.size() > 0 && positive.size() > 0) {
        correct = true;
        KdTree two = KdTree(positive, level + 1);
        KdTree one = KdTree(negative, level + 1);
        mChildrens.push_back(one);
        mChildrens.push_back(two);
        for (const auto &e : mid) {
          mElements.push_back(e);
        }
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

KdTree KdTree::getChildren(int i) const { return mChildrens[i]; }
int KdTree::getQttyChildrens() const { return mChildrens.size(); }
const Mesh *KdTree::getElement(int i) const { return mElements[i]; }
int KdTree::getQttyElements() const { return mElements.size(); }
bool KdTree::isLeaf() const { return mChildrens.size() == 0; }
int KdTree::getLevel() const { return mLevel; }
