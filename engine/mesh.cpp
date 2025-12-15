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
{


}

void Mesh::render(const glm::mat4& modelViewMatrix) {
    /// @brief carichiamo la matrice di modelView
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(modelViewMatrix));


    if (material_) {
        material_->render(modelViewMatrix);
    } else {
        /* @details
         * in caso che la mesh non abbia un materiale
         * evita che venga utilizzato il materiale
         * della mesh precedente
         */
        constexpr float gray[] = { 0.5f, 0.5f, 0.5f, 1.0f };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);
    }


    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < vertexes_.size(); ++i) {

        /// @brief applicazione della normale
        if (!normals_.empty() && i < normals_.size())
            glNormal3fv(glm::value_ptr(normals_[i]));

        /// @brief applica le coordinate uv se presenti
        if (!uv_coords_.empty() && i < uv_coords_.size())
            glTexCoord2fv(glm::value_ptr(uv_coords_[i]));

        glVertex3fv(glm::value_ptr(vertexes_[i]));
    }
    glEnd();

    /**
     * @brief reset dello stato per la prossima mesh
     * @details dopo che l'oggetto è stato disegnato
     * ripuliamo il buffer, in questo modo se il material
     * non ha una texture associata, non gli verrà
     * applicata un'altra texture
     */
    glBindTexture(GL_TEXTURE_2D, 0);
}
