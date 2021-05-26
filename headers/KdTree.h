#ifndef _KDTREE_INCLUDE
#define _KDTREE_INCLUDE

#include "Mesh.h"
#include <glm/glm.hpp>
#include <list>
#include <vector>

namespace Axis {
inline static const int DIM = 3;
inline static const glm::vec3 X = {1.0f, 0.0f, 0.0f};
inline static const glm::vec3 Y = {0.0f, 1.0f, 0.0f};
inline static const glm::vec3 Z = {0.0f, 0.0f, 1.0f};
static int axis(int i) { return i % DIM; }
static const glm::vec3 next(int i) {
  switch (axis(i)) {
  default:
  case 0:
    return Z;
  case 1:
    return X;
  case 2:
    return Y;
  }
}
}; // namespace Axis

class KdTree {
public:
  KdTree(int level = 0);
  KdTree(const std::list<Mesh *> &elements, int level = 0);
  inline static const int VECT_SIZE = 2;
  void deleteElement(const Mesh *mesh);
  void addElement(const Mesh *);

  KdTree getChildren(int i) const;
  int getQttyChildrens() const;
  const Mesh *getElement(int i) const;
  int getQttyElements() const;
  bool isLeaf() const;
  int getLevel() const;

private:
  std::vector<KdTree> mChildrens;
  std::vector<const Mesh *> mElements;
  glm::vec3 mAxis;
  glm::vec3 mPoss;
  int mLevel;
  int mAxisID;
};

#endif
