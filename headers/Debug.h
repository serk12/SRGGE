#ifndef _DEBUG_INCLUDE
#define _DEBUG_INCLUDE

#include <glm/glm.hpp>

#include "TileMapLoader.h"

struct Vertex;
class Octree;
class KdTree;
class TriangleMesh;

struct Debug {
  static void error(const std::string &s);
  static void info(const std::string &s);
  static void log(const float fps, const int triangles);
  static void print(const TileRowModels &t);
  static void print(const TileMapModels &t);
  static void print(const glm::vec3 &v);
  static void print(const std::vector<glm::vec3> &v);
  static void print(const Octree &o);
  static void print(const glm::vec4 &v);
  static void print(const std::string &s);
  static void print(const float &f);
  static void print(const KdTree &kdtree);
  static void cumulative(int n);
  static void printQtty();
  static void print(const int i);
  static void print(const std::vector<Vertex> &vec);

private:
  inline static int qtty = 0;
  inline static bool fileOpen = false;
  static std::string filePath;
  static const std::string filePathDefault;
};

#endif // ifndef _DEBUG_INCLUDE
