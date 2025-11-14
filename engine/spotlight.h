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

   class ENG_API Spotlight : public Light {

   public:
      Spotlight() noexcept;
      Spotlight(const glm::vec3& direction,
                float cutoff,
                AttenuationMode attenuationMode,
                float attenuationValue) noexcept;

      Spotlight(const Spotlight& other);
      Spotlight(Spotlight&& other) noexcept;

      Spotlight& operator=(const Spotlight& other);
      Spotlight& operator=(Spotlight&& other) noexcept;

      virtual ~Spotlight() noexcept;

      glm::vec3 getDirection() const noexcept;
      float getCutoff() const noexcept;
      AttenuationMode getAttenuationMode() const noexcept;
      float getAttenuationValue() const noexcept;

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
