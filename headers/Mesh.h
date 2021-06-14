#ifndef _MESH_INCLUDE
#define _MESH_INCLUDE

#include <glm/glm.hpp>
#include <map>
#include <string>

class TriangleMesh;
class ShaderProgram;

enum Collision { Positive = 1, Middle = 0, Negative = -1 };

class Mesh {
public:
  Mesh(glm::vec3 pos = {0, 0, 0});
  Mesh(const std::string &name, glm::vec3 pos = {0, 0, 0});

  const glm::mat4 &getModelMatrix() const;
  const std::string getName() const;
  glm::vec3 getPos() const;
  glm::vec3 getSize() const;
  glm::vec3 getMin() const;

  TriangleMesh *getTriangleMesh() const { return mModel; }

  void setInsideFrustum(bool inside);
  bool isInsideFrustum() const;
  void setOcclusion(bool occluded);
  bool stillVisible();
  bool isVisible() const;
  bool canAddToKdTree() const;
  void buildCube(glm::vec3 pos = {0, 0, 0}, glm::vec3 size = {1, 1, 1});
  Collision planeTest(const glm::vec4 &plane, bool sphere = true) const;
  int getTriangleSize() const;
  float getMaxBenefit(const glm::vec3 &player) const;
  void decreaseLOD();
  void increaseLOD();

  void render() const;
  void renderBoundinBox() const;

  static void setShaderProgram(ShaderProgram *msBasicProgram);

private:
  static void loadMesh(const std::string &fn, bool isLOD = false);
  static TriangleMesh *getMesh(const std::string &fn);
  inline static std::map<std::string, TriangleMesh *> msNameToModel;
  inline static std::map<std::string, int> msNameToMaxLOD;
  inline static ShaderProgram *msBasicProgram = nullptr;
  static const int OCCLUDED_FRAMES;
  TriangleMesh *mGround;
  TriangleMesh *mModel;
  TriangleMesh *mBoundinBox;
  std::string mName;

  glm::mat4 mModelMatrix;
  glm::vec3 mPos;
  int mLastFrameVisible, mLOD, mMaxLOD;
  mutable int mQttyTriangles;
  bool mInsideFrustum, mOccluded;
};

#endif
