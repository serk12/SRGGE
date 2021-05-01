#ifndef _OCTEE_INCLUDE
#define _OCTEE_INCLUDE

#include <glm/glm.hpp>
#include <list>
#include <utility>
#include <variant>
#include <vector>

enum Behaviour { Singleton, Discrete };

struct Vertex {
  glm::vec3 vertex;
  int index;
};

struct OctreeInfo {
  Behaviour behaviour;
  glm::vec3 max, min;
  float dis;
  int qtty;
};

class Octree {
public:
  Octree();
  ~Octree();
  Octree(const std::vector<glm::vec3> &vertices);
  Octree(OctreeInfo *oi, int i, glm::vec3 p, glm::vec3 s);
  bool add(const glm::vec3 &vertice, const int index);
  glm::vec3 getElementVec(int i) const;
  int getElementIndex(int i) const;
  int getQttyElements() const;
  int getQtty() const;
  const Octree &getChildren(int i) const;

  inline static const int VECT_SIZE = 8;
  inline static const int DIM_SIZE = 3;
  inline static const Behaviour DEFAULT_BEHAVIOUR = Behaviour::Singleton;

private:
  void add(Vertex &vertex);
  glm::ivec3 getDpos() const;
  glm::ivec3 getDsize() const;
  bool needsDivision(const glm::vec3 &vertice) const;
  bool checkError(const glm::vec3 &vertex) const;
  int vertexToIndex(const glm::vec3 &vertex) const;
  glm::vec3 indexToVertex(int i) const;

  std::vector<Octree> childrens;
  std::vector<Vertex> elements;
  OctreeInfo *octreeInfo;
  glm::vec3 pos, size;
  int qtty;
};

#endif //_OCTEE_INCLUDE
