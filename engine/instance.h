#pragma once
#include "node.h"

/**
 * @class Instance
 * @brief Rappresenta la tupla nodo e world matrix
 */
namespace Eng {
   class ENG_API Instance final {

   public:
      /// @brief Costruttori, distruttore, operatori di copia e spostamento
      Instance() noexcept = default;
      Instance(const std::shared_ptr<Node>& node, const glm::mat4& worldMatrix);

      ~Instance() noexcept = default;

      Instance(const Instance& other) = default;
      Instance(Instance&& other) noexcept = default;

      Instance& operator=(const Instance& other) = default;
      Instance& operator=(Instance&& other) noexcept = default;

      /// @brief getters e setters
      [[nodiscard]] std::shared_ptr<Node> getNodePtr() const noexcept { return node_ptr_; }
      [[nodiscard]] glm::mat4 getNodeWorldMatrix() const noexcept { return nodeWorldMatrix_; }

      void setNodePtr(const std::shared_ptr<Node>& node) noexcept { node_ptr_ = node; }
      void setNodeWorldMatrix(const glm::mat4& worldMatrix) noexcept { nodeWorldMatrix_ = worldMatrix; }

      /// @brief Overload per confronto (usato per remove in list)
      bool operator==(const Instance& other) const noexcept {
         return node_ptr_ == other.node_ptr_ && nodeWorldMatrix_ == other.nodeWorldMatrix_;
      }

   protected:
      /// @brief Puntatore al nodo
      std::shared_ptr<Node> node_ptr_;
      /// @brief World matrix del nodo
      glm::mat4 nodeWorldMatrix_;
   };
}
