#include "material.h"


using namespace Eng;

Material::Material(const glm::vec4& emission,
                           const glm::vec4& ambient,
                           const glm::vec4& diffuse,
                           const glm::vec4& specular,
                           float shininess/*,
                           Texture texture*/)
   : emission_{emission}, ambient_{ambient}, diffuse_{diffuse}, specular_{specular}, shininess_{shininess}/*, texture_{std::move(texture)}*/ {}


void Material::render(const glm::mat4& modelViewMatrix) {}
