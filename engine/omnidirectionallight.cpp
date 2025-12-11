#include "omnidirectionallight.h"
#include <GL/freeglut.h>

#include "light_number_exception.h"
#include "glm/gtc/type_ptr.hpp"

namespace Eng {

   OmnidirectionalLight::OmnidirectionalLight(const int index) noexcept
   : Light(index) {
      setActive(true);
   }


   std::shared_ptr<OmnidirectionalLight> OmnidirectionalLight::createOmnidirectionalLight() {

      if (lightNumber_ < 8) {
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

      if (!isActive()) {
         glDisable(id);
         return;
      }

      glEnable(id);

      const auto ambient = glm::vec4(ambient_, 1.0f);
      const auto diffuse = glm::vec4(diffuse_, 1.0f);
      const auto specular = glm::vec4(specular_, 1.0f);

      const float linearAttenuation = 4.5f / (radius_ > 0.0f ? radius_ : 1.0f);


      const glm::vec4 posEye = modelViewMatrix * lightPosition_;



      glLightfv(id, GL_AMBIENT,  glm::value_ptr(ambient));
      glLightfv(id, GL_DIFFUSE,  glm::value_ptr(diffuse));
      glLightfv(id, GL_SPECULAR, glm::value_ptr(specular));
      glLightfv(id, GL_POSITION, glm::value_ptr(posEye));
      glLightf(id, GL_LINEAR_ATTENUATION, linearAttenuation);

   }
}