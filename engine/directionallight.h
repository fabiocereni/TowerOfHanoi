#pragma once
#include "light.h"

namespace Eng {

   class ENG_API DirectionalLight final : public Light {
   public:

   ~DirectionalLight() override;

   void render(const glm::mat4& modelViewMatrix) override;

   static std::shared_ptr<DirectionalLight> createDirectionalLight();

   void setDirection(const glm::vec3& d) noexcept { direction_ = d; }

   [[nodiscard]] glm::vec3 getDirection() const noexcept { return direction_; }




   private:

      explicit DirectionalLight(int index) noexcept;




      glm::vec3 direction_{0,0,-1};
   };
}
