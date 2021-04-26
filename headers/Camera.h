#ifndef _CAMERA_INCLUDE
#define _CAMERA_INCLUDE

#include <glm/glm.hpp>

// Camera contains the properies of the camera the scene is using
// It is responsible for computing the associated GL matrices

class Camera {
public:
  Camera();
  ~Camera();

  glm::mat4 &getProjectionMatrix();
  glm::mat4 &getViewMatrix();

protected:
  void init(float initAngleX = 0.0f, float initAngleY = 0.0f);

  void resizeCameraViewport(int width, int height);
  void rotateCamera(float xRotation, float yRotation);
  void displaceCamera(const glm::vec3 &disp);

protected:
  float angleX, angleY, distance; // Camera parameters

private:
  void computeViewMatrix();

private:
  glm::vec3 pos;
  float rangeDistanceCamera[2];
  glm::mat4 projection, view; // OpenGL matrices
};

#endif // _CAMERA_INCLUDE
