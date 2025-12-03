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

    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < vertexes_.size(); ++i) {

        if (!normals_.empty() && i < normals_.size())
            glNormal3fv(glm::value_ptr(normals_[i]));

        glVertex3fv(glm::value_ptr(vertexes_[i]));
    }
    glEnd();
}

