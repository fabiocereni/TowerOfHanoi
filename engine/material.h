#pragma once

#include <glm/glm.hpp>
#include "texture.h"

namespace Eng {
   class ENG_API Material final : public Object {


   public:
      Material() noexcept = default;
      Material(const glm::vec4& emission,
               const glm::vec4& ambient,
               const glm::vec4& diffuse,
               const glm::vec4& specular,
               float shininess/*, Texture texture*/);

      ~Material() noexcept override = default;

      Material(const Material& other) = default;
      Material(Material&& other) noexcept = default;

      Material& operator=(const Material& other) = default;
      Material& operator=(Material&& other) noexcept = default;

      void render(const glm::mat4& parentMatrix, const glm::mat4& viewMatrix) override;

      [[nodiscard]] glm::vec4 getEmission() const noexcept {return emission_;};
      [[nodiscard]] glm::vec4 getAmbient() const noexcept {return ambient_;};
      [[nodiscard]] glm::vec4 getDiffuse() const noexcept {return diffuse_;};
      [[nodiscard]] glm::vec4 getSpecular() const noexcept {return specular_;};
      [[nodiscard]] float getShininess() const noexcept {return shininess_;};
      //[[nodiscard]] Texture getTexture() const noexcept {return texture_;};

      void setEmission(const glm::vec4& emission) noexcept {emission_ = emission;};
      void setAmbient(const glm::vec4& ambient) noexcept {ambient_ = ambient;};
      void setDiffuse(const glm::vec4& diffuse) noexcept {diffuse_ = diffuse;};
      void setSpecular(const glm::vec4& specular) noexcept {specular_ = specular;};
      void setShininess(float shininess) noexcept {shininess_ = shininess;};
      //void setTexture(const Texture& texture) noexcept {texture_ = texture;};

   protected:
      glm::vec4 emission_;
      glm::vec4 ambient_;
      glm::vec4 diffuse_;
      glm::vec4 specular_;
      float shininess_;
      //Texture texture_;
   };
}
