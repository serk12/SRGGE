#include "Camera.h"
#define GLM_FORCE_RADIANS
#include "Debug.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define PI 3.14159f

Camera::Camera() {}

Camera::~Camera() {}

void Camera::init(float initAngleX, float initAngleY) {
  pos = glm::vec3(0.0f, 0.8f, 0.0f);
  angleX = initAngleX;
  angleY = initAngleY;
  computeViewMatrix();
}

std::vector<glm::vec4> Camera::getFrustum() {
  std::vector<glm::vec4> result(6);
  glm::mat4 m = getProjectionMatrix() * getViewMatrix();
  // right
  result[0] = glm::vec4(m[0][3] + m[0][0], m[1][3] + m[1][0], m[2][3] + m[2][0],
                        m[3][3] + m[3][0]);
  // left
  result[1] = glm::vec4(m[0][3] - m[0][0], m[1][3] - m[1][0], m[2][3] - m[2][0],
                        m[3][3] - m[3][0]);
  // top
  result[2] = glm::vec4(m[0][3] - m[0][1], m[1][3] - m[1][1], m[2][3] - m[2][1],
                        m[3][3] - m[3][1]);
  // bottom
  result[3] = glm::vec4(m[0][3] + m[0][1], m[1][3] + m[1][1], m[2][3] + m[2][1],
                        m[3][3] + m[3][1]);
  // far
  result[4] = glm::vec4(m[0][2], m[1][2], m[2][2], m[3][2]);
  // near
  result[5] = glm::vec4(m[0][3] - m[0][2], m[1][3] - m[1][2], m[2][3] - m[2][2],
                        m[3][3] - m[3][2]);
  return result;
}

void Camera::resizeCameraViewport(int width, int height) {
  projection = glm::perspective(60.f / 180.f * PI, float(width) / float(height),
                                0.01f, 100.0f);
}

void Camera::rotateCamera(float xRotation, float yRotation) {
  angleX += xRotation;
  angleX = glm::max(-75.0f, glm::min(angleX, 75.0f));
  angleY += yRotation;
  computeViewMatrix();
}

void Camera::displaceCamera(const glm::vec3 &disp) {
  glm::mat4 auxDisp = glm::mat4(1.0f);
  auxDisp =
      glm::rotate(auxDisp, angleY / 180.f * PI, glm::vec3(0.0f, -1.0f, 0.0f));
  glm::vec4 aux = auxDisp * glm::vec4(disp, 1.0f);
  pos += glm::vec3(aux / aux[3]);
  computeViewMatrix();
}

void Camera::computeViewMatrix() {
  view = glm::mat4(1.0f);
  view = glm::rotate(view, angleX / 180.f * PI, glm::vec3(1.0f, 0.0f, 0.0f));
  view = glm::rotate(view, angleY / 180.f * PI, glm::vec3(0.0f, 1.0f, 0.0f));
  view = glm::translate(view, pos);
}

glm::mat4 &Camera::getProjectionMatrix() { return projection; }

glm::mat4 &Camera::getViewMatrix() { return view; }
