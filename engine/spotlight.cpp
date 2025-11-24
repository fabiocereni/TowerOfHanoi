#include "spotlight.h"

using namespace Eng;

ENG_API Spotlight::Spotlight(const glm::vec3& direction,
                             const float cutoff,
                             const AttenuationMode attenuationMode,
                             const float attenuationValue) noexcept
    : direction_{direction}, cutoff_{cutoff}, attenuationMode_{attenuationMode}, attenuationValue_{attenuationValue} {}

ENG_API Spotlight::~Spotlight() noexcept = default;

ENG_API glm::vec3 Spotlight::getDirection() const noexcept {
   return direction_;
}

ENG_API float Spotlight::getCutoff() const noexcept {
   return cutoff_;
}

ENG_API AttenuationMode Spotlight::getAttenuationMode() const noexcept {
   return attenuationMode_;
}

ENG_API float Spotlight::getAttenuationValue() const noexcept {
   return attenuationValue_;
}

ENG_API void Spotlight::setDirection(const glm::vec3& dir) noexcept {
   direction_ = dir;
}

ENG_API void Spotlight::setCutoff(const float cutoff) noexcept {
   cutoff_ = cutoff;
}

ENG_API void Spotlight::setAttenuationMode(const AttenuationMode mode) noexcept {
   attenuationMode_ = mode;
}

ENG_API void Spotlight::setAttenuationValue(const float value) noexcept {
   attenuationValue_ = value;
}

ENG_API void Spotlight::render(const glm::mat4 &C) {}
