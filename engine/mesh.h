#pragma once
#include <vector>
#include "node.h"

namespace Eng {
   class ENG_API Mesh final : public Node {

   public:
      Mesh() noexcept = default;
      ~Mesh() noexcept override = default;
      Mesh(const Mesh& other) = default;
      Mesh(Mesh&& other) noexcept = default;
      Mesh& operator=(const Mesh& other) = default;
      Mesh& operator=(Mesh&& other) noexcept = default;

      void render() override;

   protected:
      std::vector<glm::vec3> vertexes_;
   };
}
