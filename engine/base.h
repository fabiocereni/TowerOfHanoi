#pragma once
#include <memory>
#include "engine.h"


/**
 * @class Base
 */
namespace Eng {
   class ENG_API Base {
   public:
      Base() noexcept;
      ~Base() noexcept;
      Base(const Base&) = delete;
      Base& operator=(const Base&) = delete;

      static Base& getInstance() noexcept;

      bool init() noexcept;
      bool free() noexcept;

   protected:
      struct Reserved;
      std::unique_ptr<Reserved> reserved_;
      unsigned int windowId_;
      Base myself_;
   };
}
