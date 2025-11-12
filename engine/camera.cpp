#include "camera.h"
#include <iostream>
#include <source_location>

using namespace Eng;

ENG_API Camera::Camera() : projectionMatrix(1.0f) {
#ifdef _DEBUG
   std::cout << "[+] " << std::source_location::current().function_name() << " invoked\n";
#endif
}

ENG_API Camera::~Camera() {
#ifdef _DEBUG
   std::cout << "[-] " << std::source_location::current().function_name() << " invoked\n";
#endif
}

ENG_API Camera::Camera(const Camera& other)
   : Node(other), projectionMatrix(other.projectionMatrix) {
}

ENG_API Camera::Camera(Camera&& other) noexcept
   : Node(std::move(other)), projectionMatrix(other.projectionMatrix) {
}

ENG_API Camera& Camera::operator=(const Camera& other) {
   if (this != &other) {
      Node::operator=(other);
      projectionMatrix = other.projectionMatrix;
   }
   return *this;
}

ENG_API Camera& Camera::operator=(Camera&& other) noexcept {
   if (this != &other) {
      Node::operator=(std::move(other));
      projectionMatrix = other.projectionMatrix;
   }
   return *this;
}

ENG_API void Camera::render() {
#ifdef _DEBUG
   std::cout << "[Camera::render] Rendering camera '" << name << "'\n";
#endif
}

ENG_API void Camera::setProjectionMatrix(const glm::mat4& matrix) { projectionMatrix = matrix; }

ENG_API glm::mat4 Camera::getProjectionMatrix() const { return projectionMatrix; }

ENG_API glm::mat4 Camera::computeCameraInverse() const {
   return glm::inverse(projectionMatrix * matrix);
}