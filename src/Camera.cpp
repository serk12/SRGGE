#include "Camera.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "Debug.h"

#define PI 3.14159f


Camera::Camera() {}

Camera::~Camera() {}


void Camera::init(float initAngleX, float initAngleY)
{
    pos                    = glm::vec3(0.0f, 0.8f, 0.0f);
    angleX                 = initAngleX;
    angleY                 = initAngleY;
    rangeDistanceCamera[0] = 1.0f;
    rangeDistanceCamera[1] = 3.0f;
    computeViewMatrix();
}

void Camera::resizeCameraViewport(int width, int height)
{
    projection = glm::perspective(60.f / 180.f * PI, float(width) / float(height), 0.01f, 100.0f);
}

void Camera::rotateCamera(float xRotation, float yRotation)
{
    angleX += xRotation;
    angleX  = glm::max(-75.0f, glm::min(angleX, 75.0f));
    angleY += yRotation;
    computeViewMatrix();
}

void Camera::displaceCamera(const glm::vec3& disp)
{
    glm::mat4 auxDisp = glm::mat4(1.0f);
    auxDisp = glm::rotate(auxDisp, angleY / 180.f * PI, glm::vec3(0.0f, -1.0f, 0.0f));
    glm::vec4 aux = auxDisp * glm::vec4(disp, 1.0f);
    pos += glm::vec3(aux / aux[3]);
    computeViewMatrix();
}

void Camera::computeViewMatrix()
{
    view = glm::mat4(1.0f);
    view = glm::rotate(view, angleX / 180.f * PI, glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, angleY / 180.f * PI, glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, pos + glm::vec3(0.0f, distance, 0.0f));
}

glm::mat4& Camera::getProjectionMatrix()
{
    return projection;
}

glm::mat4& Camera::getViewMatrix()
{
    return view;
}
