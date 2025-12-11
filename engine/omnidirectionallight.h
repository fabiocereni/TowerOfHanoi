#pragma once
#include "light.h"

namespace Eng {

   class ENG_API OmnidirectionalLight final : public Light {
   public:

      ~OmnidirectionalLight() override;

      void render(const glm::mat4& modelViewMatrix) override;


      static std::shared_ptr<OmnidirectionalLight> createOmnidirectionalLight();

      glm::vec4 getPosition() { return lightPosition_; }

      void setPosition(glm::vec4 position) { lightPosition_ = position; }


   private:

      explicit OmnidirectionalLight(int index) noexcept;

      glm::vec4 lightPosition_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
   };

}
