#pragma once
#include "node.h"

namespace Eng {
   class ENG_API Instance final {

   public:
      Instance() noexcept = default;
      Instance(const std::shared_ptr<Node>& node, const glm::mat4& worldMatrix);

      ~Instance() noexcept = default;

      Instance(const Instance& other) = default;
      Instance(Instance&& other) noexcept = default;

      Instance& operator=(const Instance& other) = default;
      Instance& operator=(Instance&& other) noexcept = default;

      [[nodiscard]] std::shared_ptr<Node> getNodePtr() const noexcept;
      [[nodiscard]] glm::mat4 getNodeWorldMatrix() const noexcept;

      void setNodePtr(const std::shared_ptr<Node>& node) noexcept;
      void setNodeWorldMatrix(const glm::mat4& worldMatrix) noexcept;

      bool operator==(const Instance& other) const noexcept {
         return node_ptr_ == other.node_ptr_ && nodeWorldMatrix_ == other.nodeWorldMatrix_;
      }

   protected:
      std::shared_ptr<Node> node_ptr_;
      glm::mat4 nodeWorldMatrix_;
   };
}
