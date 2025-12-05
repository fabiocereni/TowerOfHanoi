#include "material.h"

#include <utility>
#include <GL/freeglut.h>

#include "glm/gtc/type_ptr.hpp"

using namespace Eng;

Material::Material(const glm::vec4& emission,
                   const glm::vec4& ambient,
                   const glm::vec4& diffuse,
                   const glm::vec4& specular,
                   const float shininess,
                   std::string textureName)
   : emission_{emission}, ambient_{ambient}, diffuse_{diffuse}, specular_{specular}, shininess_{shininess},
     textureName_ {std::move(textureName)} {}


void Material::render(const glm::mat4& modelViewMatrix) {
    // 1. Imposta i parametri di illuminazione del materiale
    glMaterialfv(GL_FRONT, GL_EMISSION, glm::value_ptr(emission_));
    glMaterialfv(GL_FRONT, GL_AMBIENT,  glm::value_ptr(ambient_));
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  glm::value_ptr(diffuse_));
    glMaterialfv(GL_FRONT, GL_SPECULAR, glm::value_ptr(specular_));
    glMaterialf (GL_FRONT, GL_SHININESS, shininess_);

    // 2. Gestione Texture
    if (texture_) {
        // Se il materiale ha una texture caricata, attivala
        texture_->bind();
    } else {
        // Se non c'è texture, assicuriamoci di disattivarla per non applicare
        // texture di altri oggetti a questo
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}