#ifndef _TRIANGLE_MESH_INCLUDE
#define _TRIANGLE_MESH_INCLUDE

#include "ShaderProgram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

using namespace std;

// Class TriangleMesh renders a very simple room with textures

class TriangleMesh {
public:
  TriangleMesh(glm::vec3 position = {0, 0, 0});

  void addVertex(const glm::vec3 &position);
  void addTriangle(int v0, int v1, int v2);
  void buildCube();

  void sendToOpenGL(ShaderProgram &program);
  void render() const;
  void free();

  glm::mat4 &getModelMatrix();

private:
  vector<glm::vec3> vertices;
  vector<int> triangles;
  glm::mat4 model;
  glm::vec3 pos;

  GLuint vao;
  GLuint vbo;
  GLint posLocation, normalLocation;
};

#endif // _TRIANGLE_MESH_INCLUDE
