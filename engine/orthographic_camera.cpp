#include "orthographic_camera.h"

using namespace Eng;

Orthographic_Camera::Orthographic_Camera(const float left, const float right,
                                         const float top, const float bottom,
                                         const float nearPlane, const float farPlane) noexcept
    : left_{left}, right_{right}, top_{top}, bottom_{bottom}, nearPlane_{nearPlane}, farPlane_{farPlane}
{
    projectionMatrix_ = glm::ortho(left_, right_, bottom_, top_, nearPlane_, farPlane_);
}

void Orthographic_Camera::render(const glm::mat4& modelViewMatrix) {}


void Orthographic_Camera::onResize(const int width, const int height) noexcept {
    // rimane uguale
}