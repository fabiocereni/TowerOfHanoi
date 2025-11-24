#include "directionallight.h"

using namespace Eng;

ENG_API DirectionalLight::DirectionalLight(const std::shared_ptr<Node>& node,
                                           const glm::mat4& worldMatrix,
                                           const glm::vec4& position)
    : node_ptr_{node}, nodeWorldMatrix_{worldMatrix}, position_{position} {}

ENG_API std::shared_ptr<Node> DirectionalLight::getNodePtr() const noexcept {
    return node_ptr_;
}

ENG_API glm::mat4 DirectionalLight::getNodeWorldMatrix() const noexcept {
    return nodeWorldMatrix_;
}

ENG_API glm::vec4 DirectionalLight::getPosition() const noexcept {
    return position_;
}

ENG_API void DirectionalLight::setNodePtr(const std::shared_ptr<Node>& node) noexcept {
    node_ptr_ = node;
}

ENG_API void DirectionalLight::setNodeWorldMatrix(const glm::mat4& worldMatrix) noexcept {
    nodeWorldMatrix_ = worldMatrix;
}

ENG_API void DirectionalLight::setPosition(const glm::vec4& position) noexcept {
    position_ = position;
}

ENG_API void DirectionalLight::render(const glm::mat4 &C, glm::mat4 M) {}