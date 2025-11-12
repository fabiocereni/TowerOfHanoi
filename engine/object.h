#pragma once
#include "engine.h"
#include <string>

namespace Eng {

   class ENG_API Object {
   protected:
      unsigned long id;
      std::string name;

   public:
      Object();
      virtual ~Object();
      Object(const Object& other);
      Object(Object&& other) noexcept;
      Object& operator=(const Object& other);
      Object& operator=(Object&& other) noexcept;

      virtual void render() = 0; // importante per override in Node

      void setName(const std::string& n);
      std::string getName() const;
      unsigned long getId() const;
   };

} // namespace Eng