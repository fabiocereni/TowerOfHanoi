#pragma once
#include "engine.h"
#include <string>

namespace Eng {
   class ENG_API Object {

   public:
      Object() noexcept;
      virtual ~Object() noexcept = default;
      Object(const Object& other) = default;
      Object(Object&& other)  noexcept = default;
      Object& operator=(const Object& other) = default;
      Object& operator=(Object&& other) noexcept = default;

      virtual void render() = 0;

      void setName(const std::string& name) noexcept;
      [[nodiscard]] std::string getName() const noexcept;
      [[nodiscard]] unsigned long getId() const noexcept;
      void setId(unsigned long id) noexcept;

   protected:
      unsigned long id_;
      std::string name_;
   };
}
