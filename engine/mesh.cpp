#include "mesh.h"
#include <GL/gl.h>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace Eng;

ENG_API Mesh::Mesh(const std::vector<glm::vec3>& vertexes, const std::shared_ptr<Material>& material) noexcept
         : vertexes_{vertexes}, material_{material} {}



void Mesh::render(glm::mat4 parentMatrix) {
    // 1. Calcola la matrice finale (Matrice del padre * Matrice locale del nodo)
    glm::mat4 worldMatrix = parentMatrix * matrix_;

    // 2. Carica la matrice in OpenGL
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(worldMatrix));

    // 3. Disegna la mesh
    // Usa il materiale se presente (opzionale per ora, ma buona prassi)
    if (material_) {
        // Qui potresti settare i colori del materiale con glMaterialfv...
        // Per ora usiamo il colore del materiale come colore base
        glColor4fv(glm::value_ptr(material_->getDiffuse()));
    }
    else {
        glColor3f(1.0f, 1.0f, 1.0f); // Bianco di default
    }

    glBegin(GL_TRIANGLE_STRIP);
    for (auto& v : vertexes_) {
        // Disegna il vertice
        glVertex3fv(glm::value_ptr(v));
        // Nota: mancano le normali (glNormal3fv) ma per ora va bene così per il test
    }
    glEnd();

    // 4. (Opzionale) Se una Mesh potesse avere figli, dovresti chiamare il render anche su di loro
    for (const auto& c : childrens_) {
        c->render(worldMatrix);
    }
}
