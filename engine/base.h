#pragma once
#include <memory>
#include "engine.h"

namespace Eng {

   class ENG_API Base {
   private:
      struct Reserved;
      std::unique_ptr<Reserved> reserved;

      Base();
      ~Base();

   public:
      Base(const Base&) = delete;
      Base& operator=(const Base&) = delete;

      static Base& getInstance();

      bool init();
      bool free();
   };

}