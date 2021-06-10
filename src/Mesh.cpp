#include "Mesh.h"

#include "Debug.h"
#include "PLYReader.h"

const int Mesh::OCCLUDED_FRAMES = 4;

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
const std::string &Mesh::getName() const { return mName; }
glm::vec3 Mesh::getPos() const { return mPos; }
glm::vec3 Mesh::getSize() const { return mModel->getSize(); }
glm::vec3 Mesh::getMin() const { return mModel->getMin(); }

void Mesh::render() const {
  if (mGround != nullptr) {
    mGround->render();
  }
  if (isVisible() || mName == TileMapLoader::WALL ||
      mName == TileMapLoader::GROUND) {
    mModel->render();
  }
}

void Mesh::renderBoundinBox() const {
  if (TileMapLoader::GROUND != mName) {
    mBoundinBox->render();
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
