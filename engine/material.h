#pragma once

#include <glm/glm.hpp>
#include "texture.h"

namespace Eng {
   class Material final : public Object {


   public:
      Material() noexcept = default;
      Material(const glm::vec4& emission,
               const glm::vec4& ambient,
               const glm::vec4& diffuse,
               const glm::vec4& specular,
               float shininess, Texture texture);

      ~Material() noexcept override = default;

      Material(const Material& other) = default;
      Material(Material&& other) noexcept = default;

      Material& operator=(const Material& other) = default;
      Material& operator=(Material&& other) noexcept = default;

      void render() override;

      [[nodiscard]] glm::vec4 getEmission() const noexcept;
      [[nodiscard]] glm::vec4 getAmbient() const noexcept;
      [[nodiscard]] glm::vec4 getDiffuse() const noexcept;
      [[nodiscard]] glm::vec4 getSpecular() const noexcept;
      [[nodiscard]] float getShininess() const noexcept;
      [[nodiscard]] Texture getTexture() const noexcept;

      void setEmission(const glm::vec4& emission) noexcept;
      void setAmbient(const glm::vec4& ambient) noexcept;
      void setDiffuse(const glm::vec4& diffuse) noexcept;
      void setSpecular(const glm::vec4& specular) noexcept;
      void setShininess(float shininess) noexcept;
      void setTexture(const Texture& texture) noexcept;

   protected:
      glm::vec4 emission_;
      glm::vec4 ambient_;
      glm::vec4 diffuse_;
      glm::vec4 specular_;
      float shininess_;
      Texture texture_;
   };
}
