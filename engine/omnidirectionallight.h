#pragma once
#include "light.h"

namespace Eng {

   class ENG_API OmnidirectionalLight final : public Light {
   public:

      ~OmnidirectionalLight() override;

      void render(const glm::mat4& modelViewMatrix) override;

      static std::shared_ptr<OmnidirectionalLight> createOmnidirectionalLight();




   private:

      explicit OmnidirectionalLight(int index) noexcept;
   };

}
