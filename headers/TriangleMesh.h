#ifndef _TRIANGLE_MESH_INCLUDE
#define _TRIANGLE_MESH_INCLUDE

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

class ShaderProgram;
using namespace std;

// Class TriangleMesh renders a very simple room with textures

struct float3 {
  float x, y, z;
  float3() { x = y = z = 0; }
  float3(glm::vec3 v) {
    x = v.x;
    y = v.y;
    z = v.z;
  }
};
struct uint3 {
  uint32_t x, y, z;
};

struct int3 {
  int x, y, z;
};

struct Face {
  glm::vec3 vertex0, vertex1, vertex2;
  int index0, index1, index2;
};

inline bool operator<(const int3 &lhs, const int3 &rhs) {
  if (lhs.x != rhs.x)
    return lhs.x < rhs.x;
  if (lhs.y != rhs.y)
    return lhs.y < rhs.y;
  return lhs.z < rhs.z;
}

class TriangleMesh {
public:
  TriangleMesh();

  void addVertex(const glm::vec3 &position);
  void addTriangle(int v0, int v1, int v2);
  void buildCube(glm::vec3 pos = {0, 0, 0}, glm::vec3 size = {1, 1, 1});
  vector<Face> getFacesByVertex(int i) const;

  void sendToOpenGL(ShaderProgram &program);
  void render() const;
  void free();

  float getRadius() const;
  glm::vec3 getSize() const;
  glm::vec3 getMin() const;

  vector<glm::vec3> getVertices() const;
  vector<float3> exportVertices() const;
  vector<uint3> exportTriangles() const;

  int getVerticesSize() const;
  int getTriangleSize() const;
  void offSetY(float y);
  vector<int3> getTriangles() const;

private:
  vector<glm::vec3> vertices;
  vector<int> triangles;

  glm::vec3 bbMin, bbMax;
  float r;

  GLuint vao;
  GLuint vbo;
  GLint posLocation, normalLocation;
};

#endif // _TRIANGLE_MESH_INCLUDE
