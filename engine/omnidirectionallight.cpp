#include "omnidirectionallight.h"

using namespace Eng;

ENG_API OmnidirectionalLight::OmnidirectionalLight(const std::shared_ptr<Node>& node,
                                                   const glm::mat4& worldMatrix,
                                                   const glm::vec4& position,
                                                   const attenuation_mode_enum attenuationMode,
                                                   const float attenuationValue)
    : node_ptr_{node},
      nodeWorldMatrix_{worldMatrix},
      position_{position},
      attenuation_mode_{attenuationMode},
      attenuation_value_{attenuationValue} {}

ENG_API std::shared_ptr<Node> OmnidirectionalLight::getNodePtr() const noexcept {
   return node_ptr_;
}

ENG_API glm::mat4 OmnidirectionalLight::getNodeWorldMatrix() const noexcept {
   return nodeWorldMatrix_;
}

ENG_API glm::vec4 OmnidirectionalLight::getPosition() const noexcept {
   return position_;
}

ENG_API attenuation_mode_enum OmnidirectionalLight::getAttenuationMode() const noexcept {
   return attenuation_mode_;
}

ENG_API float OmnidirectionalLight::getAttenuationValue() const noexcept {
   return attenuation_value_;
}

ENG_API void OmnidirectionalLight::setNodePtr(const std::shared_ptr<Node>& node) noexcept {
   node_ptr_ = node;
}

ENG_API void OmnidirectionalLight::setNodeWorldMatrix(const glm::mat4& worldMatrix) noexcept {
   nodeWorldMatrix_ = worldMatrix;
}

ENG_API void OmnidirectionalLight::setPosition(const glm::vec4& position) noexcept {
   position_ = position;
}

ENG_API void OmnidirectionalLight::setAttenuationMode(attenuation_mode_enum mode) noexcept {
   attenuation_mode_ = mode;
}

ENG_API void OmnidirectionalLight::setAttenuationValue(float value) noexcept {
   attenuation_value_ = value;
}

ENG_API void OmnidirectionalLight::render() {}
