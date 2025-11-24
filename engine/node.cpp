#include "node.h"
#include <iostream>
#include <source_location>

using namespace Eng;

ENG_API Node::Node() noexcept
            : matrix_(1.0f), parent_(nullptr) {
}


ENG_API glm::mat4 Node::getMatrix() const noexcept {
   return matrix_;
}

ENG_API void Node::setMatrix(const glm::mat4& m) noexcept {
   matrix_ = m;
}

ENG_API std::shared_ptr<Node>& Node::getParent() noexcept {
   return parent_;
}

ENG_API void Node::setParent(std::shared_ptr<Node> p) noexcept {
   parent_ = std::move(p);
}

ENG_API std::vector<std::shared_ptr<Node>>& Node::getChildrens() noexcept {
   return childrens_;
}

ENG_API void Node::addChildren(std::shared_ptr<Node> child) noexcept {
   childrens_.push_back(std::move(child));
}

ENG_API std::shared_ptr<Node> Node::removeChildren(const std::string& name) {
   for (auto it = childrens_.begin(); it != childrens_.end(); ++it) {
      if ((*it)->getName() == name) {
         auto removed = std::move(*it);
         childrens_.erase(it);
         return removed;
      }
   }
   return nullptr;
}

ENG_API std::shared_ptr<Node> Node::returnChild(const std::string& name) const {
   for (auto& c : childrens_) {
      if (c->getName() == name) {
         return c;
      }
   }
   return nullptr;
}

ENG_API glm::mat4 Node::getWorldMatrix() const noexcept {
   if (parent_)
      return parent_->getWorldMatrix() * matrix_;
   return matrix_;
}

ENG_API std::shared_ptr<Node> Node::returnChild(const unsigned long& id) const {
   for (const auto& c : childrens_) {
      if (c->getId() == id)
         return c;
   }
   return nullptr;
}


ENG_API std::shared_ptr<Camera> Node::find(std::string& str) const noexcept {
   return nullptr;
}

ENG_API void Node::render(const glm::mat4 &C, glm::mat4 M) {

}

