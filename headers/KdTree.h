#ifndef _KDTREE_INCLUDE
#define _KDTREE_INCLUDE

#include "Mesh.h"
#include <glm/glm.hpp>
#include <list>
#include <stack>
#include <vector>

struct Visibility {
  bool wasVisible;
  bool opened;
};

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
  glm::vec3 getAxis() const;

  void render() const;

  void pullUpVisibility();
  const Mesh &getAABBMesh() const;
  void renderModels() const;
  void traverseNode(std::stack<KdTree *> &traversalStack);
  Visibility computeVisibility();
  void nextFrame();

private:
  std::vector<KdTree> mChildrens;
  std::vector<Mesh *> mElements;
  glm::vec3 mPoss;
  int mLevel;
  int mAxisID;

  Mesh mGlobalAABB;
  Mesh mElementsAABB;
  KdTree *mFather;
  int mLastFrameVisible;
  bool mVisible;
  inline static const int OCCLUDED_FRAMES = 8;
};

#endif
