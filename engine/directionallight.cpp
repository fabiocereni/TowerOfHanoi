#include "directionallight.h"
#include <GL/freeglut.h>

#include "glm/gtc/type_ptr.hpp"

namespace Eng {

    void DirectionalLight::render(const glm::mat4& modelViewMatrix) {

        const GLenum id = GL_LIGHT0 + getLightNumber();
        glEnable(id);

        glLightfv(id, GL_AMBIENT,  glm::value_ptr(ambient_));
        glLightfv(id, GL_DIFFUSE,  glm::value_ptr(diffuse_));
        glLightfv(id, GL_SPECULAR, glm::value_ptr(specular_));

        const glm::vec3 eyeDir = glm::mat3(modelViewMatrix) * (-direction_);
        const float pos[] = { eyeDir.x, eyeDir.y, eyeDir.z, 0.0f };

        glLightfv(id, GL_POSITION, pos);
    }

}
