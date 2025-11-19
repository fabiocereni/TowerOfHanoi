#include "material.h"

#include <utility>

using namespace Eng;

Material::Material(const glm::vec4& emission,
                           const glm::vec4& ambient,
                           const glm::vec4& diffuse,
                           const glm::vec4& specular,
                           float shininess,
                           Texture texture)
   : emission_{emission}, ambient_{ambient}, diffuse_{diffuse}, specular_{specular}, shininess_{shininess}, texture_{std::move(texture)} {}



glm::vec4 Material::getAmbient() const noexcept {
   return ambient_;
}

glm::vec4 Material::getDiffuse() const noexcept {
   return diffuse_;
}

glm::vec4 Material::getEmission() const noexcept {
   return emission_;
}

float Material::getShininess() const noexcept {
   return shininess_;
}

glm::vec4 Material::getSpecular() const noexcept {
   return specular_;
}

Texture Material::getTexture() const noexcept {
   return texture_;
}

void Material::setAmbient(const glm::vec4 &ambient) noexcept {
   ambient_ = ambient;
}

void Material::setDiffuse(const glm::vec4 &diffuse) noexcept {
   diffuse_ = diffuse;
}

void Material::setEmission(const glm::vec4 &emission) noexcept {
   emission_ = emission;
}

void Material::setShininess(float shininess) noexcept {
   shininess_ = shininess;
}

void Material::setSpecular(const glm::vec4 &specular) noexcept {
   specular_ = specular;
}

void Material::setTexture(const Texture &texture) noexcept {
   texture_ = texture;
}

void Material::render() {}
