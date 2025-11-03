#include "node.h"
#include <iostream>
#include <source_location>

using namespace Eng;

ENG_API Node::Node()
   : matrix(1.0f), parent(nullptr), children() {
#ifdef _DEBUG
   std::cout << "[+] " << std::source_location::current().function_name() << " invoked\n";
#endif
}

ENG_API Node::~Node() {
#ifdef _DEBUG
   std::cout << "[-] " << std::source_location::current().function_name() << " invoked\n";
#endif
}

ENG_API Node::Node(const Node& other)
   : Object(other), matrix(other.matrix) {
}

ENG_API Node::Node(Node&& other) noexcept
   : Object(std::move(other)), matrix(other.matrix),
   parent(std::move(other.parent)), children(std::move(other.children)) {
}

ENG_API Node& Node::operator=(const Node& other) {
   if (this != &other) {
      Object::operator=(other);
      matrix = other.matrix;
   }
   return *this;
}

ENG_API Node& Node::operator=(Node&& other) noexcept {
   if (this != &other) {
      Object::operator=(std::move(other));
      matrix = other.matrix;
      parent = std::move(other.parent);
      children = std::move(other.children);
   }
   return *this;
}

ENG_API glm::mat4 Node::getMatrix() const { return matrix; }

ENG_API void Node::setMatrix(const glm::mat4& m) { matrix = m; }

ENG_API std::unique_ptr<Node>& Node::getParent() { return parent; }

ENG_API void Node::setParent(std::unique_ptr<Node> p) { parent = std::move(p); }

ENG_API std::vector<std::unique_ptr<Node>>& Node::getChildren() { return children; }

ENG_API void Node::addChildren(std::unique_ptr<Node> child) {
   children.push_back(std::move(child));
}

ENG_API std::unique_ptr<Node> Node::removeChildren(const std::string& name_) {
   for (auto it = children.begin(); it != children.end(); ++it) {
      if ((*it)->getName() == name_) {
         auto removed = std::move(*it);
         children.erase(it);
         return removed;
      }
   }
   return nullptr;
}

ENG_API std::unique_ptr<Node> Node::returnChild(const std::string& name_) {
   for (auto& c : children) {
      if (c->getName() == name_) {
         return std::make_unique<Node>(*c);
      }
   }
   return nullptr;
}

ENG_API glm::mat4 Node::getWorldMatrix() const {
   if (parent)
      return parent->getWorldMatrix() * matrix;
   return matrix;
}