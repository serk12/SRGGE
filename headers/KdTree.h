#ifndef _KDTREE_INCLUDE
#define _KDTREE_INCLUDE

class TriangleMesh;

namespace Axis {
inline static const glm::vec3 X = {1.0f, 0.0f, 0.0f};
inline static const glm::vec3 Y = {0.0f, 1.0f, 0.0f};
inline static const glm::vec3 Z = {0.0f, 0.0f, 1.0f};
static const glm::vec3 next(int i) {
  switch (i % 3) {
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

class KdTree {
public:
  KdTree(int level = 0);
  KdTree(const std::vector<TriangleMesh *> &elements, int level = 0);
  inline static const int VECT_SIZE = 2;
  void deleteElement(const TriangleMesh *mesh);
  void addElement(const TriangleMesh *);

private:
  std::vector<KdTree> mChildrens;
  std::vector<const TriangleMesh *> mElements;
  glm::vec3 mAxis;
  float mPoss;
  int mLevel;
};

#endif
