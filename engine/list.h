#pragma once
#include <list>
#include "object.h"
#include "instance.h"

namespace Eng {
   /**
    * @class List
    * @brief Classe utilizzate per istanza da renderizzare
    */
   class ENG_API List final : public Object {

   public:
      /// @brief Costruttori, distruttore, operatore di copia e spostamento
      List() noexcept = default;
      List(const List& other) = default;

      ~List() noexcept override = default;

      List(List&& other) noexcept = default;

      List& operator=(const List& other) = default;
      List& operator=(List&& other) noexcept = default;

      /// @brief getters e setters

      [[nodiscard]] std::list<Instance> getRenderList() const noexcept {return instances_;};
      void setRenderList(const std::list<Instance>& renderList) noexcept {instances_ = renderList;};

      /// @brief Aggiunge un'istanza in testa alla lista
      void addOnTopToRenderList(const Instance& instance) noexcept {instances_.push_front(instance);};

      /// @brief Aggiunge un'istanza in coda alla lista
      void addOnBottomToRenderList(const Instance& instance) noexcept {instances_.push_back(instance);};

      /// @brief Rimuove un'istanza dalla lista
      bool removeFromRenderList(const Instance &instance) noexcept;

      /// @brief Permette di creare la render list
      void pass(const std::shared_ptr<Node>& node_ptr, glm::mat4 parentWorldMatrix = glm::mat4(1.0f));

      /// @brief Chiama il render su tutti i nodi della render list
      void render(const glm::mat4& viewMatrix) override;

      /// @brief Pulisce la render list
      void clear() noexcept {instances_.clear();};

   protected:
      /// @brief Lista di istanze da renderizzare
      std::list<Instance> instances_;
   };
}
