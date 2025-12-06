#include "directionallight.h"
#include "light_number_exception.h"
#include <GL/freeglut.h>

#include "glm/gtc/type_ptr.hpp"

namespace Eng {

    DirectionalLight::DirectionalLight(const int index) noexcept
        : Light(index) {}


    std::shared_ptr<DirectionalLight> DirectionalLight::createDirectionalLight() {

        if (lightNumber_ < 8) {
            return std::shared_ptr<DirectionalLight>(new DirectionalLight(lightNumber_++));
        }
        throw LightNumberExceededException("Maximum number of lights exceeded: (8)");
    }


    DirectionalLight::~DirectionalLight() noexcept {
        const GLenum id = GL_LIGHT0 + index_;
        glDisable(id);
        lightNumber_--;
    }


    void DirectionalLight::render(const glm::mat4& modelViewMatrix) {

        const GLenum id = GL_LIGHT0 + index_;
        glEnable(id);

        glLightfv(id, GL_AMBIENT,  glm::value_ptr(ambient_));
        glLightfv(id, GL_DIFFUSE,  glm::value_ptr(diffuse_));
        glLightfv(id, GL_SPECULAR, glm::value_ptr(specular_));

        const glm::vec3 eyeDir = glm::mat3(modelViewMatrix) * (-direction_);
        const float pos[] = { eyeDir.x, eyeDir.y, eyeDir.z, 0.0f };

        glLightfv(id, GL_POSITION, pos);
    }

}
