#pragma once
#include <string>
#include "glm/glm.hpp"
#include "engine_api.h"

namespace Eng {
   class ENG_API Object {

   public:
      Object() noexcept;
      virtual ~Object() noexcept = default;
      Object(const Object& other) = default;
      Object(Object&& other)  noexcept = default;
      Object& operator=(const Object& other) = default;
      Object& operator=(Object&& other) noexcept = default;


      void setName(const std::string& name) noexcept {name_ = name;};
      [[nodiscard]] std::string getName() const noexcept {return name_;};
      [[nodiscard]] unsigned long getId() const noexcept {return id_;};
      void setId(unsigned long id) noexcept {id_ = id;};


      virtual void render(const glm::mat4& parentMatrix, const glm::mat4& viewMatrix) = 0;


   protected:
      unsigned long id_;
      std::string name_;
   };
}
