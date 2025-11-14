#pragma once
#include <list>

#include "object.h"
#include "instance.h"

namespace Eng {
   class ENG_API List : public Object {

   public:
      List() noexcept;
      List(const List& other);

      virtual ~List() noexcept;

      List(List&& other) noexcept;

      List& operator=(const List& other);
      List& operator=(List&& other) noexcept;

      std::list<Instance> getRenderList() const noexcept;
      void setRenderList(const std::list<Instance>& renderList) noexcept;

      void addToRenderList(const Instance& instance) noexcept;
      void removeFromRenderList(Instance& instance) noexcept;

   protected:
      std::list<Instance> instances_;
   };
}
