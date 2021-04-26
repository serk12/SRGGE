#ifndef _OCTEE_INCLUDE
#define _OCTEE_INCLUDE

#include <glm/glm.hpp>
#include <utility>

typedef std::pair<glm::vec3, int> Vertex;

class Octree {
public:
  Octree();
  Octree(const std::vector<glm::vec3> &vertices, unsigned int LOD);
  void add(const glm::vec3 vertex, const int index);
  void setQtty(unsigned int qtty);

private:
  std::vector<Octee> childrens;
  std::vector<Vertex> elements;
  glm::vec3 size;
  unsigned int qtty;
};

#endif //_OCTEE_INCLUDE
