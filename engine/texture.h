#pragma once
#include "object.h"

namespace Eng {
   class ENG_API Texture final : public Object {

   public:
      Texture() noexcept = default;
      ~Texture() noexcept override = default;
      Texture(const Texture& other) = default;
      Texture(Texture&& other) noexcept = default;
      Texture& operator=(const Texture& other) = default;
      Texture& operator=(Texture&& other) noexcept = default;

      void render(const glm::mat4 &C) override;

   protected:
      // da definire

   };
}