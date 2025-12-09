#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "texture.h"

/**
 * @class Material
 * @brief Rappresenta una generica mesh
 */
namespace Eng {
   class ENG_API Material final : public Object {


   public:
      /// @brief Costruttori, distruttore, operatori di copia e spostamento
      Material(const glm::vec4& emission,
               const glm::vec4& ambient,
               const glm::vec4& diffuse,
               const glm::vec4& specular,
               float shininess,
               std::string textureName);

      ~Material() noexcept override = default;

      Material(const Material& other) = default;
      Material(Material&& other) noexcept = default;

      Material& operator=(const Material& other) = default;
      Material& operator=(Material&& other) noexcept = default;

      void render(const glm::mat4& modelViewMatrix) override;

      /// @brief getters e setters
      [[nodiscard]] glm::vec4 getEmission() const noexcept {return emission_;};
      [[nodiscard]] glm::vec4 getAmbient() const noexcept {return ambient_;};
      [[nodiscard]] glm::vec4 getDiffuse() const noexcept {return diffuse_;};
      [[nodiscard]] glm::vec4 getSpecular() const noexcept {return specular_;};
      [[nodiscard]] float getShininess() const noexcept {return shininess_;};
      [[nodiscard]] std::shared_ptr<Texture>  getTexture() const noexcept {return texture_;};
      [[nodiscard]] std::string getTextureName() const noexcept {return textureName_;};

      void setEmission(const glm::vec4& emission) noexcept {emission_ = emission;};
      void setAmbient(const glm::vec4& ambient) noexcept {ambient_ = ambient;};
      void setDiffuse(const glm::vec4& diffuse) noexcept {diffuse_ = diffuse;};
      void setSpecular(const glm::vec4& specular) noexcept {specular_ = specular;};
      void setShininess(const float shininess) noexcept {shininess_ = shininess;};
      void setTexture(const std::shared_ptr<Texture> & texture) noexcept {texture_ = texture;};
      void setTextureName(const std::string& textureName) noexcept {textureName_ = textureName;};

   protected:
      /// @brief Componente emissiva del materiale
      glm::vec4 emission_;
      /// @brief Componente ambient del materiale
      glm::vec4 ambient_;
      /// @brief Componente diffusiva del materiale
      glm::vec4 diffuse_;
      /// @brief Componente speculare del materiale
      glm::vec4 specular_;
      /// @brief Lucentezza del materiale
      float shininess_;
      /// @brief Texture associata al materiale
      std::shared_ptr<Texture> texture_;
      /// @brief Nome della texture
      /// @details serve per binding
      std::string textureName_;
   };
}
