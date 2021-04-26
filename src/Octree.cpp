#include "Octree.h"

Octree::Octree() {}

Octree::Octree(const std::vector<glm::vec3> &vertices, unsigned int qtty) {
  glm::vec3 max, min;
  max = min = vertices[0];
  for (auto v : vertices) {
    for (unsigned int i = 0; i < 3; ++i) {
      if (v[i] < min[i])
        min[i] = v[i];
      if (v[i] > max[i])
        max[i] = v[i];
    }
  }
  size = max - min;
  qtty = qtty;

  for (unsigned int i = 0; i < vertices.size(); ++i) {
    add(vertices[i], i);
  }
}

void Octree::setQtty(unsigned int qtty) { qtty = qtty; }

void Octree::add(const glm::vec3 vertex, const int index) {}
