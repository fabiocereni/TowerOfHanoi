#pragma once
#include "engine.h"
#include <string>
#include "glm/glm.hpp"

namespace Eng {
   class ENG_API Object {

   public:
      Object() noexcept;
      virtual ~Object() noexcept = default;
      Object(const Object& other) = default;
      Object(Object&& other)  noexcept = default;
      Object& operator=(const Object& other) = default;
      Object& operator=(Object&& other) noexcept = default;

      virtual void render(const glm::mat4 &C, glm::mat4 M = glm::mat4(1.0f)) = 0;

      void setName(const std::string& name) noexcept;
      [[nodiscard]] std::string getName() const noexcept;
      [[nodiscard]] unsigned long getId() const noexcept;
      void setId(unsigned long id) noexcept;

   protected:
      unsigned long id_;
      std::string name_;
   };
}
