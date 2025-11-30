#include "directionallight.h"

using namespace Eng;

DirectionalLight::DirectionalLight(const std::shared_ptr<Node>& node,
                                           const glm::mat4& worldMatrix,
                                           const glm::vec4& position)
    : node_ptr_{node}, nodeWorldMatrix_{worldMatrix}, position_{position} {}

std::shared_ptr<Node> DirectionalLight::getNodePtr() const noexcept {
    return node_ptr_;
}

glm::mat4 DirectionalLight::getNodeWorldMatrix() const noexcept {
    return nodeWorldMatrix_;
}

glm::vec4 DirectionalLight::getPosition() const noexcept {
    return position_;
}

void DirectionalLight::setNodePtr(const std::shared_ptr<Node>& node) noexcept {
    node_ptr_ = node;
}

void DirectionalLight::setNodeWorldMatrix(const glm::mat4& worldMatrix) noexcept {
    nodeWorldMatrix_ = worldMatrix;
}

void DirectionalLight::setPosition(const glm::vec4& position) noexcept {
    position_ = position;
}

void DirectionalLight::render(const glm::mat4 &C, glm::mat4 M) {}