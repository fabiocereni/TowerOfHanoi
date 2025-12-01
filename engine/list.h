#pragma once
#include <list>
#include "object.h"
#include "instance.h"
#include <algorithm>

namespace Eng {
   class ENG_API List final : public Object {

   public:
      List() noexcept = default;
      List(const List& other) = default;

      ~List() noexcept override = default;

      List(List&& other) noexcept = default;

      List& operator=(const List& other) = default;
      List& operator=(List&& other) noexcept = default;

      [[nodiscard]] std::list<Instance> getRenderList() const noexcept {return instances_;};
      void setRenderList(const std::list<Instance>& renderList) noexcept {instances_ = renderList;};

      void addOnTopToRenderList(const Instance& instance) noexcept;
      void addOnBottomToRenderList(const Instance& instance) noexcept;
      bool removeFromRenderList(const Instance &instance) noexcept;

      void pass(const std::shared_ptr<Node>& node_ptr, glm::mat4 parentWorldMatrix = glm::mat4(1.0f));

      void render(const glm::mat4& viewMatrix) override;

   protected:
      std::list<Instance> instances_;
   };
}
