#pragma once
#include <vector>
#include "node.h"
#include "material.h"


namespace Eng {
   class ENG_API Mesh final : public Node {

   public:
      Mesh() noexcept = default;
      ~Mesh() noexcept override = default;
      Mesh(const Mesh& other) = default;
      Mesh(Mesh&& other) noexcept = default;
      Mesh& operator=(const Mesh& other) = default;
      Mesh& operator=(Mesh&& other) noexcept = default;

      void render(const glm::mat4 &C, glm::mat4 M = glm::mat4(1.0f)) override;

      [[nodiscard]] std::shared_ptr<Material> getMaterial() const noexcept;
      [[nodiscard]] std::vector<glm::vec3> getVertexes() const noexcept;

      void setMaterial(const std::shared_ptr<Material>& material) noexcept;
      void setVertexes(const std::vector<glm::vec3>& vertexes) noexcept;

   protected:
      std::vector<glm::vec3> vertexes_;
      std::shared_ptr<Material> material_;
   };
}
