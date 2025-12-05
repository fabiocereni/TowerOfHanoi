#include "omnidirectionallight.h"
#include <GL/freeglut.h>

#include "glm/gtc/type_ptr.hpp"

namespace Eng {

   void OmnidirectionalLight::render(const glm::mat4& modelViewMatrix) {

      const GLenum id = GL_LIGHT0 + getLightNumber();
      glEnable(id);

      glLightfv(id, GL_AMBIENT,  glm::value_ptr(ambient_));
      glLightfv(id, GL_DIFFUSE,  glm::value_ptr(diffuse_));
      glLightfv(id, GL_SPECULAR, glm::value_ptr(specular_));

      glm::vec4 posWorld = getMatrix() * glm::vec4(0,0,0,1);
      glm::vec4 posEye   = modelViewMatrix * posWorld;

      GLfloat pos[] = { posEye.x, posEye.y, posEye.z, 1.0f };
      glLightfv(id, GL_POSITION, pos);

      glLightf(id, GL_CONSTANT_ATTENUATION,  attConst_);
      glLightf(id, GL_LINEAR_ATTENUATION,    attLinear_);
      glLightf(id, GL_QUADRATIC_ATTENUATION, attQuad_);
   }

}
