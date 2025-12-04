#pragma once
#include "light.h"

namespace Eng {

   class ENG_API DirectionalLight final : public Light {
   public:
      DirectionalLight() noexcept = default;
      void render(const glm::mat4& modelViewMatrix) override;



   void setDirection(const glm::vec3& d) noexcept { direction_ = d; }

   [[nodiscard]] glm::vec3 getDirection() const noexcept { return direction_; }


   private:
      glm::vec3 direction_{0,0,-1};
   };
}
