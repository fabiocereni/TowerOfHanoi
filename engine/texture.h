#pragma once
#include "object.h"

namespace Eng {
   class ENG_API Texture final : public Object {

   public:
      Texture() noexcept;
      virtual ~Texture() noexcept;
      Texture(const Texture& other);
      Texture(Texture&& other) noexcept;
      Texture& operator=(const Texture& other);
      Texture& operator=(Texture&& other) noexcept;

      void render() override;

   protected:
      // da definire

   };
}