#pragma once
#include <stdexcept>
#include "engine_api.h"
#include <string>

namespace Eng {
   /**
    * @class LightNumberExceededException
    * @details Eccezione per prevenire di sforare
    *          il numero massimo di luci
    */
   class ENG_API LightNumberExceededException final : public std::runtime_error {
   public:
      explicit LightNumberExceededException(const std::string& msg)
          : std::runtime_error(msg) {}
   };

}


