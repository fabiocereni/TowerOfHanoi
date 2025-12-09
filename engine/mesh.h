#pragma once
#include <vector>
#include "node.h"
#include "material.h"


namespace Eng {
   class ENG_API Mesh final : public Node {

   public:
      Mesh(const std::vector<glm::vec3>& vertexes,
           const std::string& materialName,
           const std::shared_ptr<Material>& material = nullptr) noexcept;
      ~Mesh() noexcept override = default;
      Mesh(const Mesh& other) = default;
      Mesh(Mesh&& other) noexcept = default;
      Mesh& operator=(const Mesh& other) = default;
      Mesh& operator=(Mesh&& other) noexcept = default;

      void render(const glm::mat4& modelViewMatrix) override;

      [[nodiscard]] std::shared_ptr<Material> getMaterial() const noexcept {return material_;};
      [[nodiscard]] std::vector<glm::vec3> getVertexes() const noexcept {return vertexes_;};
      [[nodiscard]] std::vector<glm::vec3> getNormals() const noexcept {return normals_;};
      [[nodiscard]] std::vector<glm::vec2> getUv_Coords() const noexcept {return uv_coords_;};
      [[nodiscard]] std::string getMaterialName() const noexcept {return materialName_;};
      [[nodiscard]] bool getShadow() const noexcept { return shadow_; };



      void setMaterial(const std::shared_ptr<Material>& material) noexcept {material_ = material;};
      void setVertexes(const std::vector<glm::vec3>& vertexes) noexcept {vertexes_ = vertexes;};
      void setNormals(const std::vector<glm::vec3>& normals) noexcept {normals_ = normals;};
      void setUv_coords(const std::vector<glm::vec2>& uv_coords) noexcept {uv_coords_ = uv_coords;};
      void setMaterialName(const std::string& materialName) noexcept {materialName_ = materialName;};
      void setShadow(const bool& shadow) noexcept { shadow_ = shadow; };


   protected:
      std::vector<glm::vec3> vertexes_;
      std::shared_ptr<Material> material_;
      std::vector<glm::vec3> normals_;
      std::vector<glm::vec2> uv_coords_;
      std::string materialName_;
      bool shadow_ = true;
   };
}
