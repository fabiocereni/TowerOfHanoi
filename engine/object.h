#pragma once
#include "engine.h"
#include <string>

namespace Eng {
   class ENG_API Object {

   public:
      Object() noexcept;
      virtual ~Object() noexcept;
      Object(const Object& other);
      Object(Object&& other) noexcept;
      Object& operator=(const Object& other);
      Object& operator=(Object&& other) noexcept;

      virtual void render() = 0;

      void setName(const std::string& n) noexcept;
      std::string getName() const noexcept;
      unsigned long getId() const noexcept;
      void setId(unsigned int id) noexcept;

   protected:
      unsigned long id_;
      std::string name_;
   };
}
