#include "instance.h"

using namespace Eng;
using namespace std;

ENG_API Instance::Instance(const std::shared_ptr<Node>& node, const glm::mat4& worldMatrix)
               : node_ptr_{node}, nodeWorldMatrix_{worldMatrix} {}

ENG_API std::shared_ptr<Node> Instance::getNodePtr() const noexcept {
   return node_ptr_;
}

ENG_API glm::mat4 Instance::getNodeWorldMatrix() const noexcept {
   return nodeWorldMatrix_;
}

ENG_API void Instance::setNodePtr(const std::shared_ptr<Node>& node) noexcept {
   node_ptr_ = node;
}

ENG_API void Instance::setNodeWorldMatrix(const glm::mat4& worldMatrix) noexcept {
   nodeWorldMatrix_ = worldMatrix;
}
