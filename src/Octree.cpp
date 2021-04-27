#include "Octree.h"

Octree::~Octree() {
  for (unsigned int i = 0; i < VECT_SIZE; ++i) {
    delete childrens[i];
  }

  if (qtty == octreeInfo->info) {
    delete octreeInfo;
  }
}

Octree::Octree() {
  octreeInfo = new OctreeInfo;
  octreeInfo->behaviour = DEFAULT_BEHAVIOUR;
  octreeInfo->max = octreeInfo->min = glm::vec3(0.0f);
  octreeInfo->dis = 0.0f;
  size = pos = glm::vec3(0);
  qtty = 0;
}

Octree::Octree(OctreeInfo *oi, int i, glm::vec3 p, glm::vec3 s) {
  octreeInfo = oi;
  size = s / 2.0f;
  if (s[0] < octreeInfo->dis)
    octreeInfo->dis = s[0];

  pos = p + (size * indexToVertex(i));
  qtty = 0;
}

Octree::Octree(const std::vector<glm::vec3> &vertices) {
  max = min = vertices[0];
  for (auto v : vertices) {
    for (unsigned int i = 0; i < DIM_SIZE; ++i) {
      if (v[i] < min[i])
        min[i] = v[i];
      if (v[i] > max[i])
        max[i] = v[i];
    }
  }

  qtty = vertices.size() / DIM_SIZE;
  glm::vec3 length = octreeInfo->max - octreeInfo->min;
  float m = length[0];
  for (unsigned int i = 1; i < DIM_SIZE; ++i) {
    if (m < length[i])
      m = length[i];
  }
  octreeInfo->dis = m / qtty;
  size = glm::vec3(m);

  for (unsigned int i = 0; i < vertices.size(); ++i) {
    add(vertices[i], i);
  }
}

bool Octree::checkError(const glm::vec3 &vertex) {
  for (unsigned int i = 0; i < DIM_SIZE; ++i) {
    if (vertex[i] > max[i]) {
      return false;
    }
    if (vertex[i] < min[i]) {
      return false;
    }
  }
  return true;
}

glm::vec3 Octree::getDpos() { return pos / octreeInfo->dis; }
glm::vec3 Octree::getDsize() { return size / octreeInfo->dis; }

int Octree::vertexToIndex(const glm::vec3 &vertex) {
  glm::ivec3 d_vertex = (vertex - octreeInfo->min) / octreeInfo->dis;
  d_vertex = (d_vertex - getDpos()) - (getDsize() / 2.0f);
  return (d_vertex[2] < 0.0f) * 4 + (d_vertex[1] < 0.0f) * 2 +
         (d_vertex[0] < 0.0f);
}

glm::vec3 Octree::indexToVertex(int i) {
  glm::vec3 result = glm::vec3(0.0f);
  for (unsigned int j = 0; j < DIM_SIZE; ++j) {
    result[j] = (i % 2 == 1);
    i = i / 2;
  }
  return result;
}

bool Octree::add(const glm::vec3 &vertice, const int index) {
  if (!checkError(vertice))
    return false;
  ++octreeInfo->qtty;
  Vertex vertex;
  vertex.vertice = vertice;
  vertex.index = index;
  add(vertex);
  return true;
}

bool Octree::needsDivision(const glm::vec3 &vertice) {
  switch (octreeInfo->behaviour) {
  default:
  case Behaviour::Singleton:
    return qtty != 0;
  case Behaviour::Discrete:
    return vertexToIndex(vertice) > 0;
  }
}

void Octree::add(Vertex &vertex) {
  if (needsDivision()) {
    int i = vertexToIndex(v.vertice);
    childrens = std::vector<Octree>(VECT_SIZE, Octree(octreeInfo));
    childrens[i].add(vertex);
    for (Vertex v : elements) {
      i = vertexToIndex(v.vertice);
      childrens[i].add(v);
    }
    elements.clear();
  } else {
    elements.push_back(vertex);
  }
  ++qtty;
}
