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

TriangleMesh::TriangleMesh(glm::vec3 pos) : pos(pos) {
  bbMax = bbMin = glm::vec3(0.0f);
  model = glm::mat4(1.0f);
  model = glm::translate(model, pos);
}

glm::mat4 &TriangleMesh::getModelMatrix() { return model; }

void TriangleMesh::addVertex(const glm::vec3 &position) {
  if (vertices.size() == 0) {
    bbMax = bbMin = position;
  } else {
    for (unsigned int i = 0; i < 3; ++i) {
      if (bbMax[i] < position[i])
        bbMax[i] = position[i];
      if (bbMin[i] > position[i])
        bbMin[i] = position[i];
    }
    r = glm::length(bbMax - bbMin);
  }
  vertices.push_back(position);
}

glm::vec3 TriangleMesh::getPoss() const { return pos; }

BoundingBox TriangleMesh::getBoundingBox() const {
  BoundingBox bb;
  bb.pos = pos;
  bb.size = bbMax - bbMin;
  return bb;
}

float TriangleMesh::getRadius() const { return r; }

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

  int i;
  int current = this->vertices.size();
  for (i = 0; i < 8; i += 1)
    addVertex(glm::vec3(pos.x + size.x * vertices[3 * i],
                        pos.y + size.y * vertices[3 * i + 1],
                        pos.z + size.z * vertices[3 * i + 2]));
  for (i = 0; i < 12; i++)
    addTriangle(current + faces[3 * i], current + faces[3 * i + 1],
                current + faces[3 * i + 2]);
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

Collision floatToCollision(float result) {
  if (result == 0.0f) {
    return Collision::Middle;
  } else if (result > 0.0f) {
    return Collision::Positive;
  } else {
    return Collision::Negative;
  }
}

Collision planeSphereTest(const glm::vec4 &p, const glm::vec3 &poss, float r) {
  float result = glm::dot(glm::vec3(p), poss) + p.w + r;
  return floatToCollision(result);
}

Collision planeVsPointTest(const glm::vec4 &plane, const glm::vec3 &point) {
  glm::vec3 v = point - glm::vec3(-plane[4] / plane[0], 0.0f, 0.0f);
  float result = glm::dot(v, glm::vec3(plane));
  return floatToCollision(result);
}

Collision TriangleMesh::planeTest(const glm::vec4 &plane, bool sphere) const {
  if (sphere) {
    return planeSphereTest(plane, pos, r);
  } else {
    auto c = Collision::Middle;
    for (unsigned int i = 0; i < 8; ++i) {
      glm::vec3 p;
      for (unsigned int j = 0; j < 3; ++j) {
        // get the j bite from i
        p[j] = (i & (1 << j)) >> j ? bbMin[j] : bbMax[j];
      }
      auto result = planeVsPointTest(plane, p);
      if (c != Collision::Middle && result != c) {
        return Collision::Middle;
      }
      c = result;
    }
  }
}

vector<glm::vec3> TriangleMesh::getVertices() const { return vertices; }
