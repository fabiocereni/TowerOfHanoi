#include "directionallight.h"

#include <GL/freeglut.h>

#include "glm/gtc/type_ptr.inl"

using namespace Eng;


DirectionalLight::DirectionalLight(const glm::vec3& color,
                 const glm::vec3& direction) noexcept
                     : color_(color), direction_(direction) {}




void DirectionalLight::render(const glm::mat4& modelViewMatrix) {
    setPosition(glm::vec4(direction_, 0.0f));
    // da mettere la posizione
    int lightNumber = getLightNumber();
    glLightfv(GL_LIGHT2, GL_POSITION, glm::value_ptr(getPosition()));
}