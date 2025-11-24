#include "light.h"

using namespace Eng;

ENG_API Light::Light(bool smoothShading, int lightNumber, const glm::vec4& ambient,
                     const glm::vec4& diffuse, const glm::vec4& specular,
                     bool ambientEnabled, bool localViewerEnabled)
    : smoothShading_{smoothShading}, lightNumber_{lightNumber}, ambient_{ambient},
      diffuse_{diffuse}, specular_{specular}, ambientEnabled_{ambientEnabled},
      localViewerEnabled_{localViewerEnabled} {}


ENG_API bool Light::getSmoothShading() const noexcept {
   return smoothShading_;
}

ENG_API int Light::getLightNumber() const noexcept {
   return lightNumber_;
}

ENG_API glm::vec4 Light::getAmbient() const noexcept {
   return ambient_;
}

ENG_API glm::vec4 Light::getDiffuse() const noexcept {
   return diffuse_;
}

ENG_API glm::vec4 Light::getSpecular() const noexcept {
   return specular_;
}

ENG_API bool Light::isAmbientEnabled() const noexcept {
   return ambientEnabled_;
}

ENG_API bool Light::isLocalViewer() const noexcept {
   return localViewerEnabled_;
}


ENG_API void Light::setSmoothShading(const bool smoothShading) noexcept {
   smoothShading_ = smoothShading;
}

ENG_API void Light::setLightNumber(const int lightNumber) noexcept {
   lightNumber_ = lightNumber;
}

ENG_API void Light::setAmbient(const glm::vec4& ambient) noexcept {
   ambient_ = ambient;
}

ENG_API void Light::setDiffuse(const glm::vec4& diffuse) noexcept {
   diffuse_ = diffuse;
}

ENG_API void Light::setSpecular(const glm::vec4& specular) noexcept {
   specular_ = specular;
}

ENG_API void Light::setLocalViewer(const bool enabled) noexcept {
   localViewerEnabled_ = enabled;
}

ENG_API void Light::setAmbientEnabled(const bool enabled) noexcept {
   ambientEnabled_ = enabled;
}

ENG_API void Light::render(const glm::mat4 &C, glm::mat4 M) {}
