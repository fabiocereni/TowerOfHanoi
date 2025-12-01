#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "light.h"

namespace Eng {

   enum class AttenuationMode {
      Constant,
      Linear,
      Quadratic
   };

   class ENG_API Spotlight final : public Light {

   public:
      Spotlight() noexcept = default;
      Spotlight(const glm::vec3& direction,
                float cutoff,
                AttenuationMode attenuationMode,
                float attenuationValue) noexcept;

      Spotlight(const Spotlight& other) = default;
      Spotlight(Spotlight&& other) noexcept = default;

      Spotlight& operator=(const Spotlight& other) = default;
      Spotlight& operator=(Spotlight&& other) noexcept = default;

      void render(const glm::mat4& modelViewMatrix) override;

      ~Spotlight() noexcept override;

      [[nodiscard]] glm::vec3 getDirection() const noexcept;
      [[nodiscard]] float getCutoff() const noexcept;
      [[nodiscard]] AttenuationMode getAttenuationMode() const noexcept;
      [[nodiscard]] float getAttenuationValue() const noexcept;

      void setDirection(const glm::vec3& dir) noexcept;
      void setCutoff(float cutoff) noexcept;
      void setAttenuationMode(AttenuationMode mode) noexcept;
      void setAttenuationValue(float value) noexcept;

   protected:
      glm::vec3 direction_;
      float cutoff_;
      AttenuationMode attenuationMode_;
      float attenuationValue_;
   };

}
