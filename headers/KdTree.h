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
  KdTree(KdTree *father, int level);
  ~KdTree();
  KdTree(const std::list<Mesh *> &elements, KdTree *father = nullptr,
         int level = 0);
  inline static const int VECT_SIZE = 2;
  inline static int MAX_DEEP = 0;
  void deleteElement(const Mesh *mesh);
  void addElement(const Mesh *);

  KdTree *getChildren(int i) const;
  int getQttyChildrens() const;
  const Mesh *getElement(int i) const;
  int getQttyElements() const;
  bool isLeaf() const;
  int getLevel() const;
  glm::vec3 getAxis() const;

  void render(ShaderProgram &basicProgram, int level = 0) const;

  void pullUpVisibility();
  const Mesh &getAABBMesh() const;
  void renderModels(ShaderProgram &basicProgram) const;
  void traverseNode(std::stack<KdTree *> &traversalStack,
                    ShaderProgram &basicProgram);
  Visibility computeVisibility(unsigned int frame);
  void nextFrame();

private:
  std::vector<KdTree *> mChildrens;
  std::vector<Mesh *> mElements;
  glm::vec3 mPoss;
  int mLevel;
  int mAxisID;

  Mesh mGlobalAABB;
  Mesh mElementsAABB;
  KdTree *mFather;
  unsigned int mLastFrameVisible;
  bool mVisible;
  static const int OCCLUDED_FRAMES;
};

#endif
