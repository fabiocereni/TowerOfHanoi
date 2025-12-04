#include "spotlight.h"
#include <GL/freeglut.h>

#include "glm/gtc/type_ptr.hpp"

namespace Eng {

   void Spotlight::render(const glm::mat4& modelViewMatrix) {

      const GLenum id = GL_LIGHT0 + getLightNumber();
      glEnable(id);

      glLightfv(id, GL_AMBIENT,  glm::value_ptr(ambient_));
      glLightfv(id, GL_DIFFUSE,  glm::value_ptr(diffuse_));
      glLightfv(id, GL_SPECULAR, glm::value_ptr(specular_));

      const glm::vec4 posWorld = getMatrix() * glm::vec4(0,0,0,1);
      const glm::vec4 posEye   = modelViewMatrix * posWorld;

      const float pos[] = { posEye.x, posEye.y, posEye.z, 1.0f };
      glLightfv(id, GL_POSITION, pos);

      // world coordinates
      const glm::vec3 dirWorld = glm::mat3(getMatrix()) * direction_;
      // eye coordinates
      glm::vec3 dirEye    = glm::mat3(modelViewMatrix) * dirWorld;

      glLightfv(id, GL_SPOT_DIRECTION, glm::value_ptr(dirEye));

      glLightf(id, GL_SPOT_CUTOFF,   cutoff_);
      glLightf(id, GL_SPOT_EXPONENT, exponent_);

      glLightf(id, GL_CONSTANT_ATTENUATION,  attConst_);
      glLightf(id, GL_LINEAR_ATTENUATION,    attLinear_);
      glLightf(id, GL_QUADRATIC_ATTENUATION, attQuad_);
   }

}
