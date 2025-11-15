#include "orthographic_camera.h"

using namespace Eng;

ENG_API Orthographic_Camera::Orthographic_Camera(const float left, const float right,
                  const float top, const float bottom,
                  const float nearPlane, const float farPlane) noexcept
    : left_{left}, right_{right}, top_{top}, bottom_{bottom},
      nearPlane_{nearPlane}, farPlane_{farPlane}
   {
    projectionMatrix_ = glm::ortho(left_, right_, bottom_, top_, nearPlane_, farPlane_);
   }


ENG_API float Orthographic_Camera::getBottom() const noexcept {
   return bottom_;
}

ENG_API float Orthographic_Camera::getFarPlane() const noexcept {
   return farPlane_;
}

ENG_API float Orthographic_Camera::getNearPlane() const noexcept {
   return nearPlane_;
}

ENG_API float Orthographic_Camera::getLeft() const noexcept {
   return left_;
}

ENG_API float Orthographic_Camera::getRight() const noexcept {
   return right_;
}

ENG_API float Orthographic_Camera::getTop() const noexcept {
   return top_;
}

ENG_API void Orthographic_Camera::setBottom(const float bottom) noexcept {
   bottom_ = bottom;
}

ENG_API void Orthographic_Camera::setFarPlane(const float farPlane) noexcept {
   farPlane_ = farPlane;
}

ENG_API void Orthographic_Camera::setLeft(const float left) noexcept {
   left_ = left;
}

ENG_API void Orthographic_Camera::setNearPlane(const float nearPlane) noexcept {
   nearPlane_ = nearPlane;
}

ENG_API void Orthographic_Camera::setTop(const float top) noexcept {
   top_ = top;
}

ENG_API void Orthographic_Camera::setRight(const float right) noexcept {
   right_ = right;
}


ENG_API void Orthographic_Camera::render() {

}
