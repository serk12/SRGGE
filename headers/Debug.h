#ifndef _DEBUG_INCLUDE
#define _DEBUG_INCLUDE

#include "KdTree.h"
#include "Octree.h"
#include "TileMapLoader.h"
#include "TriangleMesh.h"
#include <iostream>

struct Debug {
  static void error(const std::string &s);
  static void print(const TileRowModels &t);
  static void print(const TileMapModels &t);
  static void print(const glm::vec3 &v);
  static void print(const std::vector<glm::vec3> &v);
  static void print(const Octree &o);
  static void print(const glm::vec4 &v);
  static void print(const std::string &s);
  static void print(const float &f);
  static void print(const KdTree &kdtree);
};

#endif // ifndef _DEBUG_INCLUDE
