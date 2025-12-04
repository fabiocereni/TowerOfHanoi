#pragma once
#include "light.h"

namespace Eng {

   class ENG_API OmnidirectionalLight final : public Light {
   public:
      OmnidirectionalLight() noexcept = default;
      void render(const glm::mat4& modelViewMatrix) override;
   };

}
