#include "node.h"

#include <GL/freeglut.h>

#include "glm/gtc/type_ptr.hpp"

using namespace Eng;

Node::Node() noexcept
            : matrix_(1.0f), parent_(nullptr) {
}

void Node::render(const glm::mat4& modelViewMatrix) {}


/**
 * @brief calcola la worldMatrix del nodo
 * @details moltiplica la worldMatrix del parent per la sua locale
 * @return torna la worldMatrix del nodo
 */
glm::mat4 Node::getWorldMatrix() const noexcept {
   if (parent_)
      return parent_->getWorldMatrix() * matrix_;
   return matrix_;
}


/**
 * @brief rimuove un nodo
 * @param name nome del nodo da eliminare
 * @details Cerca tra i figli diretti di un nodo e se trovato elimina il
 *    figlio con il nome corrispondente
 * @return torna il nodo eliminato
 */
std::shared_ptr<Node> Node::removeChild(const std::string& name) {
   for (auto it = children_.begin(); it != children_.end(); ++it) {
      if ((*it)->getName() == name) {
         auto removed = std::move(*it);
         children_.erase(it);
         return removed;
      }
   }
   return nullptr;
}


/**
 * @brief cerca un nodo
 * @param name nome di un nodo da cercare
 * @details Cerca tra i figli diretti di un nodo e se trovato ritorna il
 *    figlio con il nome corrispondente
 * @return torna il nodo cercato
 */
std::shared_ptr<Node> Node::returnChild(const std::string& name) const {
   for (auto& c : children_) {
      if (c->getName() == name) {
         return c;
      }
   }
   return nullptr;
}





/**
 * @brief cerca un nodo
 * @param id di un nodo da cercare
 * @details Cerca tra i figli diretti di un nodo e se trovato ritorna il
 *    figlio con id corrispondente
 * @return torna il nodo cercato
 */
std::shared_ptr<Node> Node::returnChild(const unsigned long& id) const {
   for (const auto& c : children_) {
      if (c->getId() == id)
         return c;
   }
   return nullptr;
}