#pragma once
#include "engine.h"    
#include "object.h"
#include "glm/glm.hpp"
#include <vector>
#include <memory>
#include <string>

namespace Eng {
   class ENG_API Node : public Object {

   public:
      Node() noexcept;
      virtual ~Node() noexcept;
      Node(const Node& other);
      Node(Node&& other) noexcept;
      Node& operator=(const Node& other);
      Node& operator=(Node&& other) noexcept;

      virtual void render() = 0;

      glm::mat4 getMatrix() const noexcept;
      void setMatrix(const glm::mat4& matrix) noexcept;

      std::shared_ptr<Node>& getParent() noexcept;
      void setParent(std::shared_ptr<Node> parent) noexcept;

      std::vector<std::shared_ptr<Node>>& getChildren() noexcept;
      void addChildren(std::shared_ptr<Node> child) noexcept;
      std::shared_ptr<Node> removeChildren(const std::string& name);
      std::shared_ptr<Node> returnChild(const std::string& name) const;
      std::shared_ptr<Node> returnChild(const int& id) const;
      glm::mat4 getWorldMatrix() const noexcept;

   protected:
      glm::mat4 matrix_;
      std::shared_ptr<Node> parent_;
      std::vector<std::shared_ptr<Node>> children_;
   };
}
