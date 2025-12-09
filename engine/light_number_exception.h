#pragma once
#include <stdexcept>
#include "engine_api.h"
#include <string>

namespace Eng {
   class ENG_API LightNumberExceededException final : public std::runtime_error {
   public:
      explicit LightNumberExceededException(const std::string& msg)
          : std::runtime_error(msg) {}
   };

}


