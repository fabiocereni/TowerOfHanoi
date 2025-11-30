#include "spotlight.h"

using namespace Eng;

Spotlight::Spotlight(const glm::vec3& direction,
                             const float cutoff,
                             const AttenuationMode attenuationMode,
                             const float attenuationValue) noexcept
    : direction_{direction}, cutoff_{cutoff}, attenuationMode_{attenuationMode}, attenuationValue_{attenuationValue} {}

Spotlight::~Spotlight() noexcept = default;

glm::vec3 Spotlight::getDirection() const noexcept {
   return direction_;
}

float Spotlight::getCutoff() const noexcept {
   return cutoff_;
}

AttenuationMode Spotlight::getAttenuationMode() const noexcept {
   return attenuationMode_;
}

float Spotlight::getAttenuationValue() const noexcept {
   return attenuationValue_;
}

void Spotlight::setDirection(const glm::vec3& dir) noexcept {
   direction_ = dir;
}

void Spotlight::setCutoff(const float cutoff) noexcept {
   cutoff_ = cutoff;
}

void Spotlight::setAttenuationMode(const AttenuationMode mode) noexcept {
   attenuationMode_ = mode;
}

void Spotlight::setAttenuationValue(const float value) noexcept {
   attenuationValue_ = value;
}

void Spotlight::render(const glm::mat4 &C, glm::mat4 M) {}
