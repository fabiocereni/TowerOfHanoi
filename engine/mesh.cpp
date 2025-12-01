#include "mesh.h"
#include <GL/freeglut.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace Eng;

ENG_API Mesh::Mesh(const std::vector<glm::vec3>& vertexes, const std::shared_ptr<Material>& material) noexcept
         : vertexes_{vertexes}, material_{material} {}


void Mesh::render(const glm::mat4& parentMatrix, const glm::mat4& viewMatrix) {


    const glm::mat4 worldMatrix = parentMatrix * matrix_;
    glm::mat4 modelViewMatrix = viewMatrix * worldMatrix;

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(modelViewMatrix));


    glBegin(GL_TRIANGLE_STRIP);
    for (auto& v : vertexes_) {
        glVertex3fv(glm::value_ptr(v));
    }
    glEnd();



    for (const auto& c : childrens_) {
        c->render(worldMatrix, viewMatrix);
    }

}
