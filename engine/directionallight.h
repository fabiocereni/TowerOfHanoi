#pragma once
#include <glm/glm.hpp>
#include "node.h"
#include "light.h"

namespace Eng {
   class ENG_API DirectionalLight : public Light {

   public:
      DirectionalLight() noexcept;
      DirectionalLight(const std::shared_ptr<Node>& node,
                       const glm::mat4& worldMatrix,
                       const glm::vec4& position);

      DirectionalLight(const DirectionalLight& other);
      DirectionalLight(DirectionalLight&& other) noexcept;

      DirectionalLight& operator=(const DirectionalLight& other);
      DirectionalLight& operator=(DirectionalLight&& other) noexcept;

      virtual ~DirectionalLight() noexcept;

      std::shared_ptr<Node> getNodePtr() const noexcept;
      glm::mat4 getNodeWorldMatrix() const noexcept;
      glm::vec4 getPosition() const noexcept;

      void setNodePtr(const std::shared_ptr<Node>& node) noexcept;
      void setNodeWorldMatrix(const glm::mat4& worldMatrix) noexcept;
      void setPosition(const glm::vec4& position) noexcept;

   protected:
      std::shared_ptr<Node> node_ptr_;
      glm::mat4 nodeWorldMatrix_;
      glm::vec4 position_;
   };
}
