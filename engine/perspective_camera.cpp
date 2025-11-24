#include "perspective_camera.h"

#include <glm/ext/matrix_clip_space.hpp>

using namespace Eng;

ENG_API Perspective_Camera::Perspective_Camera(const float fov,
                                               const float aspectRatio,
                                               const float nearPlane,
                                               const float farPlane) noexcept
    : fov_{fov}, aspectRatio_{aspectRatio}, nearPlane_{nearPlane}, farPlane_{farPlane}
{
   projectionMatrix_ = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}


ENG_API float Perspective_Camera::getFov() const noexcept {
   return fov_;
}

ENG_API float Perspective_Camera::getFarPlane() const noexcept {
   return farPlane_;
}

ENG_API float Perspective_Camera::getNearPlane() const noexcept {
   return nearPlane_;
}

ENG_API float Perspective_Camera::getAspectRatio() const noexcept {
   return aspectRatio_;
}

ENG_API void Perspective_Camera::setAspectRatio(const float aspectRatio) noexcept {
   aspectRatio_ = aspectRatio;
}

ENG_API void Perspective_Camera::setFarPlane(const float farPlane) noexcept {
   farPlane_ = farPlane;
}

ENG_API void Perspective_Camera::setNearPlane(const float nearPlane) noexcept {
   nearPlane_ = nearPlane;
}

ENG_API void Perspective_Camera::setFov(const float fov) noexcept {
   fov_ = fov;
}



ENG_API void Perspective_Camera::render(const glm::mat4 &C, glm::mat4 M) {

}
