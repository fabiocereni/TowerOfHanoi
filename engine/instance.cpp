#include "instance.h"

using namespace Eng;
using namespace std;

Instance::Instance(const std::shared_ptr<Node>& node, const glm::mat4& worldMatrix)
               : node_ptr_{node}, nodeWorldMatrix_{worldMatrix} {}

std::shared_ptr<Node> Instance::getNodePtr() const noexcept {
   return node_ptr_;
}

glm::mat4 Instance::getNodeWorldMatrix() const noexcept {
   return nodeWorldMatrix_;
}

void Instance::setNodePtr(const std::shared_ptr<Node>& node) noexcept {
   node_ptr_ = node;
}

void Instance::setNodeWorldMatrix(const glm::mat4& worldMatrix) noexcept {
   nodeWorldMatrix_ = worldMatrix;
}
