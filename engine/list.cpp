#include "list.h"
#include "light.h"

#include <iostream>

using namespace Eng;

void List::addOnBottomToRenderList(const Instance &instance) noexcept {
   instances_.push_back(instance);
}

void List::addOnTopToRenderList(const Instance &instance) noexcept {
   instances_.push_front(instance);
}

bool List::removeFromRenderList(const Instance &instance) noexcept {
   const size_t oldSize = instances_.size();
   instances_.remove(instance);
   return instances_.size() != oldSize;
}

void List::pass(const std::shared_ptr<Node>& node_ptr, glm::mat4 parentWorldMatrix) {

   // costruzione world matrix del nodo
   parentWorldMatrix = parentWorldMatrix * node_ptr->getMatrix();

   const auto current = std::dynamic_pointer_cast<Light>(node_ptr);
   Instance instance;
   instance.setNodePtr(node_ptr);
   instance.setNodeWorldMatrix(parentWorldMatrix);

   // se è una luce
   if (current) {
      addOnTopToRenderList(instance);
   } else {
      addOnBottomToRenderList(instance);
   }


   // Recursively pass all child nodes:
   for (const auto& childNode : node_ptr.get()->getChildrens()) {
      this->pass(childNode, parentWorldMatrix);
   }

}

// il render riceve la viewMatrix e si calcola la modelViewMatrix
void List::render(const glm::mat4& viewMatrix) {

   for (const auto& instance : getRenderList()) {

      // costruzione della modelview
      glm::mat4 modelViewMatrix = viewMatrix * instance.getNodeWorldMatrix();

      // il render del node riceve e carica direttamente la modelViewMatrix
      instance.getNodePtr()->render(modelViewMatrix);
   }
}


void List::clear() noexcept {
   instances_.clear();
}
