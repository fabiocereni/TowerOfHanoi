#pragma once
#include <glm/glm.hpp>
#include "node.h"
#include "light.h"

namespace Eng {
   class ENG_API DirectionalLight final : public Light {

   public:
      DirectionalLight() noexcept = default;
      DirectionalLight(const std::shared_ptr<Node>& node,
                       const glm::mat4& worldMatrix,
                       const glm::vec4& position);

      DirectionalLight(const DirectionalLight& other) = default;
      DirectionalLight(DirectionalLight&& other) noexcept = default;

      DirectionalLight& operator=(const DirectionalLight& other) = default;
      DirectionalLight& operator=(DirectionalLight&& other) noexcept = default;

      ~DirectionalLight() noexcept override = default;

      void render(const glm::mat4& modelViewMatrix) override;

      [[nodiscard]] std::shared_ptr<Node> getNodePtr() const noexcept;
      [[nodiscard]] glm::mat4 getNodeWorldMatrix() const noexcept;
      [[nodiscard]] glm::vec4 getPosition() const noexcept;

      void setNodePtr(const std::shared_ptr<Node>& node) noexcept;
      void setNodeWorldMatrix(const glm::mat4& worldMatrix) noexcept;
      void setPosition(const glm::vec4& position) noexcept;

   protected:
      std::shared_ptr<Node> node_ptr_;
      glm::mat4 nodeWorldMatrix_;
      glm::vec4 position_;
   };
}
