#pragma once
#include "glm/glm.hpp"
#include <vector>
#include <memory>
#include <string>

#include "object.h"

namespace Eng {
   class ENG_API Node : public Object {

   public:
      Node() noexcept;
      ~Node() noexcept override = default;
      Node(const Node& other) = default;
      Node(Node&& other) noexcept = default;
      Node& operator=(const Node& other) = default;
      Node& operator=(Node&& other) noexcept = default;

      void render(const glm::mat4& modelViewMatrix) override;

      [[nodiscard]] glm::mat4 getMatrix() const noexcept {return matrix_;};
      void setMatrix(const glm::mat4& matrix) noexcept {matrix_ = matrix;};

      std::shared_ptr<Node>& getParent() noexcept {return parent_;};
      void setParent(const std::shared_ptr<Node>& parent) noexcept {parent_ = parent;};

      std::vector<std::shared_ptr<Node>>& getChildrens() noexcept {return childrens_;};
      void addChildren(const std::shared_ptr<Node>& child) noexcept {childrens_.push_back(child);};

      std::shared_ptr<Node> removeChildren(const std::string& name);
      [[nodiscard]] std::shared_ptr<Node> returnChild(const std::string& name) const;
      [[nodiscard]] std::shared_ptr<Node> returnChild(const unsigned long& id) const;
      [[nodiscard]] glm::mat4 getWorldMatrix() const noexcept;

      //[[nodiscard]] std::shared_ptr<Node> find(const std::string& str) const noexcept;

   protected:
      glm::mat4 matrix_;
      std::shared_ptr<Node> parent_;
      std::vector<std::shared_ptr<Node>> childrens_;
   };
}
