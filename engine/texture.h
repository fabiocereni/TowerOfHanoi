#pragma once
#include "object.h"

namespace Eng {
   class ENG_API Texture final : public Object {

   public:
      Texture(const unsigned char* bitmap, int width, int height) noexcept;
      ~Texture() noexcept override;
      Texture(const Texture& other) = default;
      Texture(Texture&& other) noexcept = default;
      Texture& operator=(const Texture& other) = default;
      Texture& operator=(Texture&& other) noexcept = default;

      void render(const glm::mat4& parentMatrix, const glm::mat4& viewMatrix) override;

      void bind() const;

   protected:
      unsigned int texId_;
      int width_;
      int height_;
   };
}