#include "omnidirectionallight.h"

using namespace Eng;

OmnidirectionalLight::OmnidirectionalLight(const std::shared_ptr<Node>& node,
                                                   const glm::mat4& worldMatrix,
                                                   const glm::vec4& position,
                                                   const attenuation_mode_enum attenuationMode,
                                                   const float attenuationValue)
    : node_ptr_{node},
      nodeWorldMatrix_{worldMatrix},
      position_{position},
      attenuation_mode_{attenuationMode},
      attenuation_value_{attenuationValue} {}

std::shared_ptr<Node> OmnidirectionalLight::getNodePtr() const noexcept {
   return node_ptr_;
}

glm::mat4 OmnidirectionalLight::getNodeWorldMatrix() const noexcept {
   return nodeWorldMatrix_;
}

glm::vec4 OmnidirectionalLight::getPosition() const noexcept {
   return position_;
}

attenuation_mode_enum OmnidirectionalLight::getAttenuationMode() const noexcept {
   return attenuation_mode_;
}

float OmnidirectionalLight::getAttenuationValue() const noexcept {
   return attenuation_value_;
}

void OmnidirectionalLight::setNodePtr(const std::shared_ptr<Node>& node) noexcept {
   node_ptr_ = node;
}

void OmnidirectionalLight::setNodeWorldMatrix(const glm::mat4& worldMatrix) noexcept {
   nodeWorldMatrix_ = worldMatrix;
}

void OmnidirectionalLight::setPosition(const glm::vec4& position) noexcept {
   position_ = position;
}

void OmnidirectionalLight::setAttenuationMode(attenuation_mode_enum mode) noexcept {
   attenuation_mode_ = mode;
}

void OmnidirectionalLight::setAttenuationValue(float value) noexcept {
   attenuation_value_ = value;
}

void OmnidirectionalLight::render(const glm::mat4& modelViewMatrix) {}
