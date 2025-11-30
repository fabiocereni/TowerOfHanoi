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

      void render(const glm::mat4 &C, glm::mat4 M = glm::mat4(1.0f)) override;

      [[nodiscard]] glm::mat4 getMatrix() const noexcept;
      void setMatrix(const glm::mat4& matrix) noexcept;

      std::shared_ptr<Node>& getParent() noexcept;
      void setParent(std::shared_ptr<Node> parent) noexcept;

      std::vector<std::shared_ptr<Node>>& getChildrens() noexcept;
      void addChildren(const std::shared_ptr<Node>& child) noexcept;
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
