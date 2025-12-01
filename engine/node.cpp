#include "node.h"

#include <GL/freeglut.h>
#include "glm/gtc/type_ptr.hpp"

using namespace Eng;

Node::Node() noexcept
            : matrix_(1.0f), parent_(nullptr) {
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


ENG_API void Node::render(const glm::mat4& parentMatrix, const glm::mat4& viewMatrix) {

   const glm::mat4 worldMatrix = parentMatrix * matrix_;
   glm::mat4 modelViewMatrix = viewMatrix * worldMatrix;

   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixf(glm::value_ptr(modelViewMatrix));

   // chiamare qua eventuali metodi per disegnare la mesh

   for (const auto& c : childrens_) {
      c->render(worldMatrix, viewMatrix);
   }

}


// std::shared_ptr<Node> Node::find(const std::string& str) const noexcept {
//    return nullptr;
// }


