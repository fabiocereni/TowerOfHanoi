#pragma once
#include "glm/glm.hpp"
#include <vector>
#include <memory>
#include <string>

#include "object.h"


/**
 * @class Node
 * @brief Rappresenta un generico nodo
 */
namespace Eng {
   class ENG_API Node : public Object {

   public:
      /// @brief Costruttori, distruttore, operatori di copia e spostamento
      Node() noexcept;
      ~Node() noexcept override = default;
      Node(const Node& other) = default;
      Node(Node&& other) noexcept = default;
      Node& operator=(const Node& other) = default;
      Node& operator=(Node&& other) noexcept = default;


      void render(const glm::mat4& modelViewMatrix) override;


      /// @brief getters e setters
      [[nodiscard]] glm::mat4 getMatrix() const noexcept {return matrix_;};
      void setMatrix(const glm::mat4& matrix) noexcept {matrix_ = matrix;};
      [[nodiscard]] glm::mat4 getWorldMatrix() const noexcept;

      std::shared_ptr<Node>& getParent() noexcept {return parent_;};
      void setParent(const std::shared_ptr<Node>& parent) noexcept {parent_ = parent;};

      std::vector<std::shared_ptr<Node>>& getChildren() noexcept {return children_;};
      void addChildren(const std::shared_ptr<Node>& child) noexcept {children_.push_back(child);};

      std::shared_ptr<Node> removeChild(const std::string& name);
      void setChildren(const std::vector<std::shared_ptr<Node>>& children) noexcept {children_ = children;};
      [[nodiscard]] std::shared_ptr<Node> returnChild(const std::string& name) const;
      [[nodiscard]] std::shared_ptr<Node> returnChild(const unsigned long& id) const;



   protected:
      /// @brief Matrice locale del nodo
      glm::mat4 matrix_;
      /// @brief Puntatore al padre
      std::shared_ptr<Node> parent_;
      /// @brief Figli del node
      std::vector<std::shared_ptr<Node>> children_;
   };
}
