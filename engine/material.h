#pragma once

#include <glm/glm.hpp>
#include "texture.h"

namespace Eng {
   class Material {

   public:
      Material() noexcept;
      Material(const glm::vec4& em, const glm::vec4& am, const glm::vec4& di,
               const glm::vec4& sp, float sh, const Texture& tex);

      virtual ~Material() noexcept;

      Material(const Material& other);
      Material(Material&& other) noexcept;

      Material& operator=(const Material& other);
      Material& operator=(Material&& other) noexcept;

      glm::vec4 getEmission() const noexcept;
      glm::vec4 getAmbient() const noexcept;
      glm::vec4 getDiffuse() const noexcept;
      glm::vec4 getSpecular() const noexcept;
      float getShininess() const noexcept;
      Texture getTexture() const noexcept;

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
