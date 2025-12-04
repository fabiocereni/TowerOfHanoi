#include "mesh.h"
#include <GL/freeglut.h>

#include <utility>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace Eng;

ENG_API Mesh::Mesh(const std::vector<glm::vec3>& vertexes,
                   const std::string& materialName,
                   const std::shared_ptr<Material>& material) noexcept
   : vertexes_{vertexes},
     material_{material},
     materialName_{materialName}
{}



void Mesh::render(const glm::mat4& modelViewMatrix) {

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(modelViewMatrix));

    // --- 1. APPLICAZIONE MATERIALE ---
    if (material_) {
        material_->render(modelViewMatrix);
    } else {
        // Fallback: se non c'è materiale, usa un colore grigio standard e disattiva texture
        float gray[] = { 0.5f, 0.5f, 0.5f, 1.0f };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // --- 2. DISEGNO GEOMETRIA ---
    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < vertexes_.size(); ++i) {

        // Applica la normale (essenziale per le luci)
        if (!normals_.empty() && i < normals_.size())
            glNormal3fv(glm::value_ptr(normals_[i]));

        // NUOVO: Applica le coordinate Texture (UV) se presenti
        if (!uv_coords_.empty() && i < uv_coords_.size())
            glTexCoord2fv(glm::value_ptr(uv_coords_[i]));

        glVertex3fv(glm::value_ptr(vertexes_[i]));
    }
    glEnd();

    // Pulizia: disabilita texture dopo il render
    glBindTexture(GL_TEXTURE_2D, 0);
}

