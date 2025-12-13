#include "light.h"

namespace Eng {

int Light::lightNumber_ = 0;


   Light::Light(const int index) noexcept
      : index_(index) {
      setActive(true);
   }


}
