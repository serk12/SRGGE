#ifndef _MESH_INCLUDE
#define _MESH_INCLUDE

#include "TileMapLoader.h"
#include "TriangleMesh.h"
#include <glm/glm.hpp>

enum Collision { Positive = 1, Middle = 0, Negative = -1 };

class Mesh {
public:
  Mesh(glm::vec3 pos = {0, 0, 0});
  Mesh(const std::string &name, glm::vec3 pos = {0, 0, 0});

  const glm::mat4 &getModelMatrix() const;
  const std::string &getName() const;
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

  void render() const;
  void renderBoundinBox() const;

  static void setShaderProgram(ShaderProgram *msBasicProgram);

private:
  static void loadMesh(const std::string &fn);
  static TriangleMesh *getMesh(const std::string &fn);
  inline static std::map<std::string, TriangleMesh *> msNameToModel;
  inline static ShaderProgram *msBasicProgram = nullptr;
  static const int OCCLUDED_FRAMES;
  TriangleMesh *mGround;
  TriangleMesh *mModel;
  TriangleMesh *mBoundinBox;
  std::string mName;

  glm::mat4 mModelMatrix;
  glm::vec3 mPos;
  int mLastFrameVisible = 0;
  bool mInsideFrustum, mOccluded;
};

#endif
