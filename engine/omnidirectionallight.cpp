#include "omnidirectionallight.h"
#include <GL/freeglut.h>

#include "light_number_exception.h"
#include "glm/gtc/type_ptr.hpp"

namespace Eng {

   OmnidirectionalLight::OmnidirectionalLight(const int index) noexcept
   : Light(index) {}


   std::shared_ptr<OmnidirectionalLight> OmnidirectionalLight::createOmnidirectionalLight() {

      if (lightNumber_ < 8) {
         lightNumber_++;
         return std::shared_ptr<OmnidirectionalLight>(new OmnidirectionalLight(lightNumber_++));
      }
      throw LightNumberExceededException("Maximum number of lights exceeded: (8)");
   }

   OmnidirectionalLight::~OmnidirectionalLight() {
      const GLenum id = GL_LIGHT0 + index_;
      glDisable(id);
      lightNumber_--;
   }


   void OmnidirectionalLight::render(const glm::mat4& modelViewMatrix) {

      const GLenum id = GL_LIGHT0 + index_;
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
