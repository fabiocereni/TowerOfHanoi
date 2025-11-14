#pragma once
#include "node.h"

namespace Eng {
   class ENG_API Instance {

   public:
      Instance() noexcept;
      Instance(const std::shared_ptr<Node>& node, const glm::mat4& worldMatrix);

      virtual ~Instance() noexcept;

      Instance(const Instance& other);
      Instance(Instance&& other) noexcept;

      Instance& operator=(const Instance& other);
      Instance& operator=(Instance&& other) noexcept;

      std::shared_ptr<Node> getNodePtr() const noexcept;
      glm::mat4 getNodeWorldMatrix() const noexcept;

      void setNodePtr(const std::shared_ptr<Node>& node) noexcept;
      void setNodeWorldMatrix(const glm::mat4& worldMatrix) noexcept;

   protected:
      std::shared_ptr<Node> node_ptr_;
      glm::mat4 nodeWorldMatrix_;
   };
}
