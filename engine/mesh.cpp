#include "mesh.h"
#include <GL/gl.h>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace Eng;

ENG_API Mesh::Mesh(const std::vector<glm::vec3>& vertexes, const std::shared_ptr<Material>& material) noexcept
         : vertexes_{vertexes}, material_{material} {}



void Mesh::render(const glm::mat4 &C, glm::mat4 M) {


    // glPushMatrix();
    glMultMatrixf(glm::value_ptr(M));

    glBegin(GL_TRIANGLE_STRIP);
    for (auto& v : vertexes_) {
        glVertex3fv(glm::value_ptr(v));
    }
    glEnd();

    // glPopMatrix();



}
