#include "camera.h"

using namespace Eng;

glm::mat4 Camera::getViewMatrix() const noexcept {
    return glm::inverse(getWorldMatrix());
}
