#include "list.h"

using namespace Eng;

std::list<Instance> List::getRenderList() const noexcept {
   return instances_;
}

void List::setRenderList(const std::list<Instance> &renderList) noexcept {
   instances_ = renderList;
}

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

void List::pass(const std::shared_ptr<Node>& node_ptr, glm::mat4 matrix = glm::mat4(1.0f)) {

   // Update current transformation:
   matrix = matrix * node_ptr->getMatrix();


   // Append this node to the list:
   Instance instance;
   instance.setNodePtr(node_ptr);
   instance.setNodeWorldMatrix(matrix);
   this->instances_.push_back(instance);


   // Recursively pass all child nodes:
   for (const auto& childNode : node_ptr.get()->getChildrens()) {
      this->pass(childNode, matrix);
   }

}


// void List::render(const glm::mat4 &C, glm::mat4 M) const {
//
//    // Render each instance by passing its modelview as param:
//    for (auto &instance : this->instances_) {
//       glm::mat4 modelview = C * instance.getNodeWorldMatrix();
//       instance.getNodePtr()->render(modelview);
//    }
// }
