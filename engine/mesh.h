#pragma once
#include <vector>
#include "node.h"

namespace Eng {
   class ENG_API Mesh final : public Node {

   public:
      Mesh() noexcept;
      virtual ~Mesh() noexcept;
      Mesh(const Mesh& other);
      Mesh(Mesh&& other) noexcept;
      Mesh& operator=(const Mesh& other);
      Mesh& operator=(Mesh&& other) noexcept;

      virtual void render() override;

   protected:
      std::vector<glm::vec3> vertexes_;
   };
}
