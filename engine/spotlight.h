#pragma once
#include "light.h"

namespace Eng {

   class ENG_API Spotlight final : public Light {
   public:

      ~Spotlight() override;

      void render(const glm::mat4& modelViewMatrix) override;

      void setCutoff   (const float c) noexcept { cutoff_ = c; }
      void setExponent (const float e) noexcept { exponent_ = e; }
      void setDirection(const glm::vec3& d) noexcept { direction_ = d; }


      [[nodiscard]] float getCutoff()   const noexcept { return cutoff_; }
      [[nodiscard]] float getExponent() const noexcept { return exponent_; }
      [[nodiscard]] glm::vec3 getDirection() const noexcept { return direction_; }


      static std::shared_ptr<Spotlight> createSpotLight();


   private:

      explicit Spotlight(int index) noexcept;


      glm::vec3 direction_{0,0,-1};
      float cutoff_   = 180.0f;
      float exponent_ = 0.0f;
   };
}