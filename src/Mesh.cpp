#include "Mesh.h"

#include "Debug.h"
#include "PLYReader.h"

void Mesh::loadMesh(const std::string &fn) {
  TriangleMesh *mesh = new TriangleMesh();
  PLYReader reader;
  bool bSuccess = reader.readMesh(fn, *mesh);
  if (bSuccess) {
    if (fn == TileMapLoader::GROUND) {
      mesh->offSetY(-0.5f);
    }
    mesh->sendToOpenGL(*msBasicProgram);
    msNameToModel[fn] = mesh;
  }
}

void Mesh::setShaderProgram(ShaderProgram *basicProgram) {
  msBasicProgram = basicProgram;
}

TriangleMesh *Mesh::getMesh(const std::string &fn) {
  if (msNameToModel.find(fn) == msNameToModel.end()) {
    loadMesh(fn);
  }
  return msNameToModel[fn];
}

Mesh::Mesh(const std::string &name, glm::vec3 pos) {
  mName = name;
  mModelMatrix = glm::mat4(1.0f);
  mPos = pos;
  mModelMatrix = glm::translate(mModelMatrix, mPos);
  if (mName != TileMapLoader::WALL && mName != TileMapLoader::GROUND) {
    mGround = getMesh(TileMapLoader::GROUND);
  } else {
    mGround = nullptr;
  }
  mModel = getMesh(name);
}

glm::mat4 &Mesh::getModelMatrix() { return mModelMatrix; }
glm::vec3 Mesh::getPos() const { return mPos; }
glm::vec3 Mesh::getSize() const { return mModel->getSize(); }

void Mesh::render() const {
  if (mGround != nullptr) {
    mGround->render();
  }
  mModel->render();
}

inline Collision floatToCollision(float result) {
  if (result == 0.0f) {
    return Collision::Middle;
  } else if (result > 0.0f) {
    return Collision::Positive;
  } else {
    return Collision::Negative;
  }
}

inline Collision planeVsSphereTest(const glm::vec4 &p, const glm::vec3 &poss,
                                   float r) {
  float result = glm::dot(p, glm::vec4(poss, 1.0f)) + r;
  return floatToCollision(result);
}

Collision Mesh::planeTest(const glm::vec4 &plane, bool sphere) const {
  if (sphere) {
    return planeVsSphereTest(plane, mPos, mModel->getRadius());
  } else {
    // To Do plane vs BB
    return Collision::Middle;
  }
}
