#include "spotlight.h"
#include <GL/freeglut.h>

#include "light_number_exception.h"
#include "glm/gtc/type_ptr.hpp"

namespace Eng {

   Spotlight::Spotlight(const int index) noexcept
   : Light(index) {}


   std::shared_ptr<Spotlight> Spotlight::createSpotLight() {
      if (lightNumber_ < 8) {
         return std::shared_ptr<Spotlight>(new Spotlight(lightNumber_++));
      }
      throw LightNumberExceededException("Maximum number of lights exceeded: (8)");
   }


   Spotlight::~Spotlight() {
      const GLenum id = GL_LIGHT0 + index_;
      glDisable(id);
      lightNumber_--;
   }



   void Spotlight::render(const glm::mat4& modelViewMatrix) {

      const GLenum id = GL_LIGHT0 + index_;
      glEnable(id);

      const auto ambient = glm::vec4(ambient_, 1.0f);
      const auto diffuse = glm::vec4(diffuse_, 1.0f);
      const auto specular = glm::vec4(specular_, 1.0f);
      const auto direction = glm::vec4(direction_, 0.0f);
      const auto position = glm::vec4(direction_, 1.0f);

      const float constantAttenuation = 1.0f / (radius_ / 100);

      glLightfv(id, GL_AMBIENT,  glm::value_ptr(ambient));
      glLightfv(id, GL_DIFFUSE,  glm::value_ptr(diffuse));
      glLightfv(id, GL_SPECULAR, glm::value_ptr(specular));
      glLightfv(id, GL_POSITION, glm::value_ptr(position));
      glLightfv(id, GL_SPOT_DIRECTION, glm::value_ptr(direction));
      glLightf(id, GL_SPOT_CUTOFF, cutoff_);
      glLightf(id, GL_SPOT_EXPONENT, exponent_);
      glLightf(id, GL_CONSTANT_ATTENUATION,  constantAttenuation);
   }

}
