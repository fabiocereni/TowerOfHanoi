#include "camera.h"

using namespace Eng;

/**
 * @brief Calcola la view matrix
 * @details inverte la worldMatrix della camera
 * @return Ritorna la view matrix
 */
glm::mat4 Camera::getViewMatrix() const noexcept {
    return glm::inverse(getWorldMatrix());
}
