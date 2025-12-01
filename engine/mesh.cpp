#include "mesh.h"
#include <GL/gl.h>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace Eng;

ENG_API Mesh::Mesh(const std::vector<glm::vec3>& vertexes, const std::shared_ptr<Material>& material) noexcept
         : vertexes_{vertexes}, material_{material} {}



void Mesh::render(glm::mat4 parentMatrix) {


    // glPushMatrix();
    glMultMatrixf(glm::value_ptr(worldMatrix));

    glBegin(GL_TRIANGLE_STRIP);
    for (auto& v : vertexes_) {
        glVertex3fv(glm::value_ptr(v));
    }
    glEnd();

    // glPopMatrix();



}
