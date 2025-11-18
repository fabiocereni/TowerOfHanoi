#include "mesh.h"
#include <iostream>

using namespace Eng;


ENG_API std::shared_ptr<Material> Mesh::getMaterial() const noexcept {
   return material_;
}

ENG_API std::vector<glm::vec3> Mesh::getVertexes() const noexcept {
   return vertexes_;
}

ENG_API void Mesh::setMaterial(const std::shared_ptr<Material>& material) noexcept {
   material_ = material;
}

ENG_API void Mesh::setVertexes(const std::vector<glm::vec3>& vertexes) noexcept {
   vertexes_ = vertexes;
}



void setMaterial() noexcept;
void setVertexes() noexcept;

ENG_API void Mesh::render() {
#ifdef _DEBUG
   std::cout << "[Mesh::render] Rendering mesh '" << name_ << "' with " << vertexes_.size() / 3 << " vertices\n";
#endif
}