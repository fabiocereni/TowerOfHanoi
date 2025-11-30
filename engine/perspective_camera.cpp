#include "perspective_camera.h"

using namespace Eng;

Perspective_Camera::Perspective_Camera(const float fov,
                                               const float aspectRatio,
                                               const float nearPlane,
                                               const float farPlane) noexcept
    : fov_{fov}, aspectRatio_{aspectRatio}, nearPlane_{nearPlane}, farPlane_{farPlane}
{
    updateProjection();
}

void Perspective_Camera::updateProjection() noexcept {
    projectionMatrix_ = glm::perspective(glm::radians(fov_), aspectRatio_, nearPlane_, farPlane_);
}


void Perspective_Camera::render(const glm::mat4& C, glm::mat4 M) {}