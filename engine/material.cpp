#include "material.h"

#include <utility>

using namespace Eng;

ENG_API Material::Material(const glm::vec4& emission,
                           const glm::vec4& ambient,
                           const glm::vec4& diffuse,
                           const glm::vec4& specular,
                           float shininess,
                           Texture texture)
   : emission_{emission}, ambient_{ambient}, diffuse_{diffuse}, specular_{specular}, shininess_{shininess}, texture_{std::move(texture)} {}



ENG_API glm::vec4 Material::getAmbient() const noexcept {
   return ambient_;
}

ENG_API glm::vec4 Material::getDiffuse() const noexcept {
   return diffuse_;
}

ENG_API glm::vec4 Material::getEmission() const noexcept {
   return emission_;
}

ENG_API float Material::getShininess() const noexcept {
   return shininess_;
}

ENG_API glm::vec4 Material::getSpecular() const noexcept {
   return specular_;
}

ENG_API Texture Material::getTexture() const noexcept {
   return texture_;
}

ENG_API void Material::setAmbient(const glm::vec4 &ambient) noexcept {
   ambient_ = ambient;
}

ENG_API void Material::setDiffuse(const glm::vec4 &diffuse) noexcept {
   diffuse_ = diffuse;
}

ENG_API void Material::setEmission(const glm::vec4 &emission) noexcept {
   emission_ = emission;
}

ENG_API void Material::setShininess(float shininess) noexcept {
   shininess_ = shininess;
}

ENG_API void Material::setSpecular(const glm::vec4 &specular) noexcept {
   specular_ = specular;
}

ENG_API void Material::setTexture(const Texture &texture) noexcept {
   texture_ = texture;
}

ENG_API void Material::render(const glm::mat4 &C, glm::mat4 M) {}
