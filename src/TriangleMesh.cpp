#include "Debug.h"
#include "TriangleMesh.h"
#include <vector>

using namespace std;

vector<float3> TriangleMesh::exportVertices() const {
  vector<float3> result(getVerticesSize());
  int i = 0;
  for (auto &v : vertices) {
    result[i] = float3(v);
    ++i;
  }
  return result;
}

vector<Face> TriangleMesh::getFacesByVertex(int i) const {
  vector<Face> result;
  for (unsigned int j = 0; j < triangles.size(); ++j) {
    if (triangles[j] == i) {
      Face face;
      if (j % 3 == 0) {
        face.vertex0 = vertices[triangles[j]];
        face.vertex1 = vertices[triangles[j + 1]];
        face.vertex2 = vertices[triangles[j + 2]];
      } else if (j % 3 == 1) {
        face.vertex0 = vertices[triangles[j - 1]];
        face.vertex1 = vertices[triangles[j]];
        face.vertex2 = vertices[triangles[j + 1]];
      } else {
        face.vertex0 = vertices[triangles[j - 2]];
        face.vertex1 = vertices[triangles[j - 1]];
        face.vertex2 = vertices[triangles[j]];
      }
      result.push_back(face);
    }
  }
  return result;
}

int TriangleMesh::getVerticesSize() const { return vertices.size(); }
int TriangleMesh::getTriangleSize() const { return triangles.size() / 3; }

vector<uint3> TriangleMesh::exportTriangles() const {
  vector<uint3> result(getTriangleSize());
  uint3 aux;
  for (unsigned int i = 0; i < triangles.size(); ++i) {
    if (i % 3 == 0) {
      aux.x = triangles[i];
    } else if (i % 3 == 1) {
      aux.y = triangles[i];
    } else {
      aux.z = triangles[i];
      result[i / 3] = aux;
    }
  }
  return result;
}

vector<int3> TriangleMesh::getTriangles() const {
  vector<int3> result(getTriangleSize());
  int3 aux;
  for (unsigned int i = 0; i < triangles.size(); ++i) {
    if (i % 3 == 0) {
      aux.x = triangles[i];
    } else if (i % 3 == 1) {
      aux.y = triangles[i];
    } else {
      aux.z = triangles[i];
      result[i / 3] = aux;
    }
  }
  return result;
}

TriangleMesh::TriangleMesh(glm::vec3 pos) : pos(pos) {
  model = glm::mat4(1.0f);
  model = glm::translate(model, pos);
}

glm::mat4 &TriangleMesh::getModelMatrix() { return model; }

void TriangleMesh::addVertex(const glm::vec3 &position) {
  vertices.push_back(position);
}

glm::vec3 TriangleMesh::getPoss() const { return pos; }
float TriangleMesh::getRadius() const { return 1.0f; }

void TriangleMesh::addTriangle(int v0, int v1, int v2) {
  triangles.push_back(v0);
  triangles.push_back(v1);
  triangles.push_back(v2);
}

void TriangleMesh::buildCube(glm::vec3 pos, glm::vec3 size) {
  float vertices[] = {0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0,
                      0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1};

  int faces[] = {3, 1, 0, 3, 2, 1, 5, 6, 7, 4, 5, 7, 7, 3, 0, 0, 4, 7,
                 1, 2, 6, 6, 5, 1, 0, 1, 4, 5, 4, 1, 2, 3, 7, 7, 6, 2};

  int current = this->vertices.size();
  for (unsigned int i = 0; i < 8; i += 1)
    addVertex(glm::vec3(pos.x + size.x * vertices[3 * i],
                        pos.y + size.y * vertices[3 * i + 1],
                        pos.z + size.z * vertices[3 * i + 2]));
  for (unsigned int i = 0; i < 12; i++) {
    addTriangle(current + faces[3 * i], current + faces[3 * i + 1],
                current + faces[3 * i + 2]);
    addTriangle(current + faces[3 * i + 2], current + faces[3 * i + 1],
                current + faces[3 * i]);
  }
}

void TriangleMesh::sendToOpenGL(ShaderProgram &program) {
  vector<float> data;

  for (unsigned int tri = 0; tri < triangles.size(); tri += 3) {
    glm::vec3 normal;

    normal =
        glm::cross(vertices[triangles[tri + 1]] - vertices[triangles[tri]],
                   vertices[triangles[tri + 2]] - vertices[triangles[tri]]);
    normal = glm::normalize(normal);
    for (unsigned int vrtx = 0; vrtx < 3; vrtx++) {
      data.push_back(vertices[triangles[tri + vrtx]].x);
      data.push_back(vertices[triangles[tri + vrtx]].y);
      data.push_back(vertices[triangles[tri + vrtx]].z);

      data.push_back(normal.x);
      data.push_back(normal.y);
      data.push_back(normal.z);
    }
  }

  // Send data to OpenGL
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0],
               GL_STATIC_DRAW);
  posLocation =
      program.bindVertexAttribute("position", 3, 6 * sizeof(float), 0);
  normalLocation = program.bindVertexAttribute("normal", 3, 6 * sizeof(float),
                                               (void *)(3 * sizeof(float)));
}

void TriangleMesh::render() const {
  glBindVertexArray(vao);
  glEnableVertexAttribArray(posLocation);
  glEnableVertexAttribArray(normalLocation);
  glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 3 * triangles.size() / 3);
}

void TriangleMesh::free() {
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);

  vertices.clear();
  triangles.clear();
}

vector<glm::vec3> TriangleMesh::getVertices() const { return vertices; }
