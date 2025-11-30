#include "light.h"

using namespace Eng;

Light::Light(bool smoothShading, int lightNumber, const glm::vec4& ambient,
                     const glm::vec4& diffuse, const glm::vec4& specular,
                     bool ambientEnabled, bool localViewerEnabled)
    : smoothShading_{smoothShading}, lightNumber_{lightNumber}, ambient_{ambient},
      diffuse_{diffuse}, specular_{specular}, ambientEnabled_{ambientEnabled},
      localViewerEnabled_{localViewerEnabled} {}


bool Light::getSmoothShading() const noexcept {
   return smoothShading_;
}

int Light::getLightNumber() const noexcept {
   return lightNumber_;
}

glm::vec4 Light::getAmbient() const noexcept {
   return ambient_;
}

glm::vec4 Light::getDiffuse() const noexcept {
   return diffuse_;
}

glm::vec4 Light::getSpecular() const noexcept {
   return specular_;
}

bool Light::isAmbientEnabled() const noexcept {
   return ambientEnabled_;
}

bool Light::isLocalViewer() const noexcept {
   return localViewerEnabled_;
}


void Light::setSmoothShading(const bool smoothShading) noexcept {
   smoothShading_ = smoothShading;
}

void Light::setLightNumber(const int lightNumber) noexcept {
   lightNumber_ = lightNumber;
}

void Light::setAmbient(const glm::vec4& ambient) noexcept {
   ambient_ = ambient;
}

void Light::setDiffuse(const glm::vec4& diffuse) noexcept {
   diffuse_ = diffuse;
}

void Light::setSpecular(const glm::vec4& specular) noexcept {
   specular_ = specular;
}

void Light::setLocalViewer(const bool enabled) noexcept {
   localViewerEnabled_ = enabled;
}

void Light::setAmbientEnabled(const bool enabled) noexcept {
   ambientEnabled_ = enabled;
}

void Light::render(const glm::mat4 &C, glm::mat4 M) {}
