#include "Debug.h"
#include "Octree.h"
#include <iostream>

Octree::~Octree() {
  childrens.clear();

  if (qtty == octreeInfo->qtty) {
    delete octreeInfo;
  }
}

Octree::Octree() {
  octreeInfo = new OctreeInfo;
  octreeInfo->behaviour = DEFAULT_BEHAVIOUR;
  octreeInfo->max = octreeInfo->min = glm::vec3(0.0f);
  octreeInfo->dis = 0.0;
  octreeInfo->qtty = 0;
  octreeInfo->levels = 1;
  size = pos = glm::vec3(0);
  qtty = level = 0;
}

Octree::Octree(OctreeInfo *oi, int i, glm::vec3 p, glm::vec3 s, int l) {
  octreeInfo = oi;
  size = s / 2.0f;
  if (s[0] < octreeInfo->dis)
    octreeInfo->dis = s[0];

  pos = p + (size * indexToVertex(i));
  qtty = 0;
  level = l + 1;
  if (octreeInfo->levels < level)
    octreeInfo->levels = level;
}

Octree::Octree(const std::vector<glm::vec3> &vertices) : Octree() {
  if (vertices.size() <= 0)
    return;
  octreeInfo->max = octreeInfo->min = vertices[0];
  for (auto v : vertices) {
    for (unsigned int i = 0; i < DIM_SIZE; ++i) {
      if (v[i] < octreeInfo->min[i])
        octreeInfo->min[i] = v[i];
      if (v[i] > octreeInfo->max[i])
        octreeInfo->max[i] = v[i];
    }
  }

  glm::vec3 length = octreeInfo->max - octreeInfo->min;
  float m = length[0];
  for (unsigned int i = 1; i < DIM_SIZE; ++i) {
    if (m < length[i])
      m = length[i];
  }
  octreeInfo->dis = m;
  size = glm::vec3(m);
  for (unsigned int i = 0; i < vertices.size(); ++i) {
    add(vertices[i], i);
  }
}

void Octree::cut(int l) {
  if (level < l) {
    for (auto &c : childrens) {
      c.cut(l);
    }
  } else if (level == l) {
    for (auto &c : childrens) {
      auto result = c.getAllElements();
      if (result.size() > 0) {
        elements.insert(elements.end(), result.begin(), result.end());
      }
    }
    childrens.clear();
  }
}

bool Octree::checkError(const glm::vec3 &vertex) const {
  for (unsigned int i = 0; i < DIM_SIZE; ++i) {
    if (vertex[i] > octreeInfo->max[i]) {
      return false;
    }
    if (vertex[i] < octreeInfo->min[i]) {
      return false;
    }
  }
  return true;
}

glm::ivec3 Octree::getDpos() const { return pos / octreeInfo->dis; }
glm::ivec3 Octree::getDsize() const { return size / octreeInfo->dis; }
int Octree::getLevel() const { return level; }
int Octree::getMaxLevel() const { return octreeInfo->levels; }

int Octree::vertexToIndex(const glm::vec3 &vertex) const {
  glm::ivec3 d_vertex = (vertex - octreeInfo->min) / octreeInfo->dis;
  d_vertex = (d_vertex - getDpos()) * 2 - getDsize();
  return (d_vertex[2] >= 0.0f) * 4 + (d_vertex[1] >= 0.0f) * 2 +
         (d_vertex[0] >= 0.0f);
}

glm::vec3 Octree::indexToVertex(int i) const {
  glm::vec3 result = glm::vec3(0.0f);
  for (unsigned int j = 0; j < DIM_SIZE; ++j) {
    result[j] = (i % 2 == 1);
    i = i / 2;
  }
  return result;
}

glm::vec3 Octree::getPoss() const { return pos; }
glm::vec3 Octree::getSize() const { return size; }
float Octree::getDis() const { return octreeInfo->dis; }

bool Octree::needsDivision(const glm::vec3 &vertice) const {
  switch (octreeInfo->behaviour) {
  default:
  case Behaviour::Singleton:
    return qtty != 0 &&
           glm::distance(vertice, elements[0].vertex) > 0.0000000001f;
  case Behaviour::Discrete:
    return vertexToIndex(vertice) > 0;
  }
}

bool Octree::add(const glm::vec3 &vertice, const int index) {
  if (!checkError(vertice))
    return false;
  ++octreeInfo->qtty;
  Vertex vertex = {vertice, index};
  add(vertex);
  return true;
}

void Octree::add(Vertex &vertex) {
  if (childrens.size() == VECT_SIZE) {
    int i = vertexToIndex(vertex.vertex);
    childrens[i].add(vertex);
  } else {
    if (needsDivision(vertex.vertex)) {
      int i = vertexToIndex(vertex.vertex);
      childrens = std::vector<Octree>(VECT_SIZE);
      for (unsigned int i = 0; i < VECT_SIZE; ++i) {
        childrens[i] = Octree(octreeInfo, i, pos, size, level);
      }
      childrens[i].add(vertex);
      for (Vertex v : elements) {
        i = vertexToIndex(v.vertex);
        childrens[i].add(v);
      }
      elements.clear();
    } else {
      elements.push_back(vertex);
    }
  }
  ++qtty;
}

std::vector<Vertex> Octree::getAllElements() const {
  std::vector<Vertex> result;
  if (childrens.size() == VECT_SIZE) {
    for (auto &c : childrens) {
      auto r = c.getAllElements();
      result.insert(result.end(), r.begin(), r.end());
    }
  } else {
    for (auto &v : elements) {
      result.push_back(v);
    }
  }
  return result;
}
glm::vec3 Octree::getElementVec(int i) const { return elements[i].vertex; }
int Octree::getElementIndex(int i) const { return elements[i].index; }
int Octree::getQttyElements() const { return elements.size(); }
int Octree::getQttyChildrens() const { return childrens.size(); }
int Octree::getQtty() const { return qtty; }
const Octree &Octree::getChildren(int i) const { return childrens[i]; }
