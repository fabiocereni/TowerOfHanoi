#pragma once
#include <list>
#include "object.h"
#include "instance.h"
#include <algorithm>

namespace Eng {
   class ENG_API List : public Object {

   public:
      List() noexcept = default;
      List(const List& other) = default;

      ~List() noexcept override = default;

      List(List&& other) noexcept = default;

      List& operator=(const List& other) = default;
      List& operator=(List&& other) noexcept = default;

      [[nodiscard]] std::list<Instance> getRenderList() const noexcept;
      void setRenderList(const std::list<Instance>& renderList) noexcept;

      void addOnTopToRenderList(const Instance& instance) noexcept;
      void addOnBottomToRenderList(const Instance& instance) noexcept;
      bool removeFromRenderList(const Instance &instance) noexcept;

   protected:
      std::list<Instance> instances_;
   };
}
