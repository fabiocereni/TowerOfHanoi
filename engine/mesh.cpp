#include "mesh.h"
#include <iostream>
#include <source_location>

using namespace Eng;

ENG_API Mesh::Mesh() : vertexes() {
#ifdef _DEBUG
   std::cout << "[+] " << std::source_location::current().function_name() << " invoked\n";
#endif
}

ENG_API Mesh::~Mesh() {
#ifdef _DEBUG
   std::cout << "[-] " << std::source_location::current().function_name() << " invoked\n";
#endif
}

ENG_API Mesh::Mesh(const Mesh& other)
   : Node(other), vertexes(other.vertexes) {
}

ENG_API Mesh::Mesh(Mesh&& other) noexcept
   : Node(std::move(other)), vertexes(std::move(other.vertexes)) {
}

ENG_API Mesh& Mesh::operator=(const Mesh& other) {
   if (this != &other) {
      Node::operator=(other);
      vertexes = other.vertexes;
   }
   return *this;
}

ENG_API Mesh& Mesh::operator=(Mesh&& other) noexcept {
   if (this != &other) {
      Node::operator=(std::move(other));
      vertexes = std::move(other.vertexes);
   }
   return *this;
}

ENG_API void Mesh::render() {
#ifdef _DEBUG
   std::cout << "[Mesh::render] Rendering mesh '" << name << "' with " << vertexes.size() / 3 << " vertices\n";
#endif
}