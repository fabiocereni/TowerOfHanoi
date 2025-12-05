#include "light.h"

namespace Eng {

int Light::lightNumber_ = 0;

    Light::Light() noexcept {
        lightNumber_++;
    }


    Light::~Light() noexcept {
        lightNumber_--;
    }

}
