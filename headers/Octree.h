#ifndef _OCTEE_INCLUDE
#define _OCTEE_INCLUDE

#include <glm/glm.hpp>
#include <list>
#include <utility>
#include <variant>
#include <vector>

enum Behaviour { Singleton, Discrete };

struct Vertex {
  const glm::vec3 vertice;
  const int index;
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
  bool add(const glm::vec3 &vertice, const int index);

private:
  void add(Vertex &vertex);
  glm::vec3 getDpos() const;
  glm::vec3 getDsize() const;
  bool needsDivision() const;
  void checkError(const glm::vec3 &vertex) const;
  int vertexToIndex(const glm::vec3 &vertex) const;
  glm::vec3 indexToVertex(int i) const;

  std::vector<Octree> childrens;
  std::list<Vertex> elements;
  OctreeInfo *octreeInfo;
  glm::vec3 pos, size;
  int qtty;

  inline static const int VECT_SIZE = 8;
  inline static const int DIM_SIZE = 3;
  inline static const Behaviour DEFAULT_BEHAVIOUR = Behaviour::Singleton;
};

#endif //_OCTEE_INCLUDE
