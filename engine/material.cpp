#include "material.h"

#include <utility>

using namespace Eng;

Material::Material(const glm::vec4& emission,
                   const glm::vec4& ambient,
                   const glm::vec4& diffuse,
                   const glm::vec4& specular,
                   const float shininess,
                   std::string textureName)
   : emission_{emission}, ambient_{ambient}, diffuse_{diffuse}, specular_{specular}, shininess_{shininess},
     textureName_ {std::move(textureName)} {}


void Material::render(const glm::mat4& modelViewMatrix) {}
