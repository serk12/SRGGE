#include "Mesh.h"

#include <assert.h>
#include <filesystem>
#include <iostream>

#include "Debug.h"
#include "PLYReader.h"
#include "ShaderProgram.h"
#include "TileMapLoader.h"
#include "TriangleMesh.h"

const int Mesh::OCCLUDED_FRAMES = 4;

void Mesh::loadMesh(const std::string &fn, bool isLOD) {
  TriangleMesh *mesh = new TriangleMesh();
  PLYReader reader;
  bool bSuccess = reader.readMesh(fn, *mesh);
  if (bSuccess) {
    if (fn == TileMapLoader::GROUND) {
      mesh->offSetY(-0.5f);
    }
    mesh->sendToOpenGL(*msBasicProgram);
    msNameToModel[fn] = mesh;
    int i = 1;
    auto name = fn;
    name.resize(name.size() - 4);
    name += "_";
    auto path = fn;
    while (path[path.size() - 1] != '/') {
      path.pop_back();
    }
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
      if (std::string(entry.path()).find(name) != std::string::npos) {
        loadMesh(std::string(entry.path()), true);
        ++i;
      }
    }
    if (!isLOD) {
      msNameToMaxLOD[fn] = i;
    }
  }
}

void Mesh::setInsideFrustum(bool inside) { mInsideFrustum = inside; }
void Mesh::setOcclusion(bool occluded) {
  mOccluded = occluded;
  if (occluded) {
    mLastFrameVisible = OCCLUDED_FRAMES;
  }
}

bool Mesh::stillVisible() {
  if (mName == TileMapLoader::WALL || mName == TileMapLoader::GROUND) {
    return false;
  }
  if (mLastFrameVisible > 0) {
    --mLastFrameVisible;
    return false;
  }
  return true;
}

bool Mesh::isVisible() const { return mInsideFrustum && !mOccluded; }
bool Mesh::isInsideFrustum() const { return mInsideFrustum; }

bool Mesh::canAddToKdTree() const { return mName != TileMapLoader::GROUND; }

void Mesh::setShaderProgram(ShaderProgram *basicProgram) {
  msBasicProgram = basicProgram;
}

TriangleMesh *Mesh::getMesh(const std::string &fn) {
  if (msNameToModel.find(fn) == msNameToModel.end()) {
    loadMesh(fn);
  }
  return msNameToModel[fn];
}

Mesh::Mesh(glm::vec3 pos) {
  mInsideFrustum = true;
  mOccluded = false;
  mName = "";
  mPos = pos;
  mLOD = mQttyTriangles = mLastFrameVisible = 0;
  mModelMatrix = glm::mat4(1.0f);
  mModelMatrix = glm::translate(mModelMatrix, mPos);
  mGround = nullptr;
  mBoundinBox = nullptr;
  mModel = new TriangleMesh();
}

Mesh::Mesh(const std::string &name, glm::vec3 pos) : Mesh(pos) {
  mName = name;
  if (mName != TileMapLoader::WALL && mName != TileMapLoader::GROUND) {
    mGround = getMesh(TileMapLoader::GROUND);
  }
  mModel = getMesh(name);
  mBoundinBox = new TriangleMesh();
  mBoundinBox->buildCube(mModel->getMin(), mModel->getSize());
  mBoundinBox->sendToOpenGL(*msBasicProgram);
}

void Mesh::buildCube(glm::vec3 pos, glm::vec3 size) {
  mModel->buildCube(pos, size);
  mModel->sendToOpenGL(*msBasicProgram);
}

const glm::mat4 &Mesh::getModelMatrix() const { return mModelMatrix; }
const std::string Mesh::getName() const { return mName; }
glm::vec3 Mesh::getPos() const { return mPos; }
glm::vec3 Mesh::getSize() const { return mModel->getSize(); }
glm::vec3 Mesh::getMin() const { return mModel->getMin(); }

void Mesh::render() const {
  mQttyTriangles = 0;
  if (mGround != nullptr) {
    mGround->render();
    mQttyTriangles += mGround->getTriangleSize();
  }
  if (isVisible() || mName == TileMapLoader::WALL ||
      mName == TileMapLoader::GROUND) {
    mModel->render();
    mQttyTriangles += mModel->getTriangleSize();
  }
}

int Mesh::getTriangleSize() const { return mQttyTriangles; }

float Mesh::getMaxBenefit(const glm::vec3 &player, bool decrease) const {

  int nextLOD = mLOD;
  if (decrease) {
    ++nextLOD;
    if (nextLOD > msNameToMaxLOD[mName] - 2)
      nextLOD = msNameToMaxLOD[mName] - 2;
  } else {
    --nextLOD;
    if (nextLOD <= 0)
      nextLOD = 0;
  }
  int cluster = msNameToMaxLOD[mName] - nextLOD - 1;
  return 1.0f - ((mModel->getRadius() * 2.0f) /
                 (pow(2, cluster) * glm::distance(player, mPos)));
}

std::string computeName(std::string name, int mLOD) {
  name.resize(name.size() - 4);
  if (mLOD != 0) {
    name += "_" + std::to_string(mLOD);
  }
  name += ".ply";
  return name;
}

void Mesh::decreaseLOD() {
  if (mLOD >= (msNameToMaxLOD[mName] - 2)) {
    return;
  } else {
    ++mLOD;
  }
  mModel = msNameToModel[computeName(mName, mLOD)];
}
void Mesh::increaseLOD() {
  if (mLOD <= 0) {
    return;
  } else {
    --mLOD;
  }
  mModel = msNameToModel[computeName(mName, mLOD)];
}

void Mesh::renderBoundinBox() const {
  mQttyTriangles = 0;
  if (TileMapLoader::GROUND != mName) {
    mBoundinBox->render();
    mQttyTriangles += mBoundinBox->getTriangleSize();
  }
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
