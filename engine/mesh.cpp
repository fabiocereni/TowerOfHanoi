#include "mesh.h"
#include <iostream>

using namespace Eng;

ENG_API void Mesh::render() {
#ifdef _DEBUG
   std::cout << "[Mesh::render] Rendering mesh '" << name_ << "' with " << vertexes_.size() / 3 << " vertices\n";
#endif
}