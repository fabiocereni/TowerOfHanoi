#include "camera.h"
#include <iostream>
#include <source_location>

using namespace Eng;

ENG_API Camera::Camera() noexcept
               : projectionMatrix_(1.0f) {
}

ENG_API void Camera::render(const glm::mat4 &C, glm::mat4 M) {
}

ENG_API void Camera::setProjectionMatrix(const glm::mat4& matrix) noexcept {
   projectionMatrix_ = matrix;
}

ENG_API glm::mat4 Camera::getProjectionMatrix() const noexcept {
   return projectionMatrix_;
}

ENG_API glm::mat4 Camera::computeCameraInverse() const noexcept {
   return glm::inverse(projectionMatrix_ * matrix_);
}