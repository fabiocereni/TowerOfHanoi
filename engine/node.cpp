#include "node.h"

#include <GL/gl.h>

#include "glm/gtc/type_ptr.hpp"

using namespace Eng;

Node::Node() noexcept
            : matrix_(1.0f), parent_(nullptr) {
}


glm::mat4 Node::getMatrix() const noexcept {
   return matrix_;
}

void Node::setMatrix(const glm::mat4& m) noexcept {
   matrix_ = m;
}

std::shared_ptr<Node>& Node::getParent() noexcept {
   return parent_;
}

void Node::setParent(std::shared_ptr<Node> p) noexcept {
   parent_ = std::move(p);
}

std::vector<std::shared_ptr<Node>>& Node::getChildrens() noexcept {
   return childrens_;
}

void Node::addChildren(const std::shared_ptr<Node>& child) noexcept {
   childrens_.push_back(child);
}

std::shared_ptr<Node> Node::removeChildren(const std::string& name) {
   for (auto it = childrens_.begin(); it != childrens_.end(); ++it) {
      if ((*it)->getName() == name) {
         auto removed = std::move(*it);
         childrens_.erase(it);
         return removed;
      }
   }
   return nullptr;
}

std::shared_ptr<Node> Node::returnChild(const std::string& name) const {
   for (auto& c : childrens_) {
      if (c->getName() == name) {
         return c;
      }
   }
   return nullptr;
}

glm::mat4 Node::getWorldMatrix() const noexcept {
   if (parent_)
      return parent_->getWorldMatrix() * matrix_;
   return matrix_;
}

std::shared_ptr<Node> Node::returnChild(const unsigned long& id) const {
   for (const auto& c : childrens_) {
      if (c->getId() == id)
         return c;
   }
   return nullptr;
}


ENG_API void Node::render(const glm::mat4 &C, glm::mat4 M) {



   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixf(glm::value_ptr(C));


   for (const auto& c : childrens_) {
      c->render(C);
   }

}


// std::shared_ptr<Node> Node::find(const std::string& str) const noexcept {
//    return nullptr;
// }


