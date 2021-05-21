#include "KdTree.h"
#include "TriangleMesh.h"

KdTree::KdTree(level) : mAxis(Axis::next(level)), mLevel(level), mPoss(0.0f) {}

KdTree::KdTree(const std::vector<TriangleMesh *> &elements, int level)
    : KdTree(level) {
  int size = mElements.size();
  if (size > 1) {
    mPoss = glm::vec3(0.0f);
    for (const auto &e : elements) {
      mPoss += e->getPoss();
    }
    mPoss *= axis / size;
    std::vector<TriangleMesh *> positive, negative;
    for (const auto &e : elements) {
      auto result = e->planeTest(glm::vec4(mAxis, mPoss));
      switch (result) {
      default:
      case Collision::Middle:
        positive.push_back(e);
        negative.push_back(e);
        break;
      case Collision::Positive:
        positive.push_back(e);
        break;
      case Collision::Negative:
        negative.push_back(e);
        break;
      }
      childrens = {KdTree(negative, level + 1), KdTree(positive, level + 1)};
    }
  } else if (size == 1) {
    mElements.push_back(elements[0]);
  }
}
