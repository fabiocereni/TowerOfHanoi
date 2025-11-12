#pragma once
#include "engine.h"
#include "object.h"
#include <glm/mat4x4.hpp>
#include <vector>
#include <memory>
#include <string>

namespace Eng {

   class ENG_API Node : public Object {
   protected:
      glm::mat4 matrix;
      std::shared_ptr<Node> parent;
      std::vector<std::shared_ptr<Node>> children;

   public:
      Node();
      virtual ~Node();
      Node(const Node& other);
      Node(Node&& other) noexcept;
      Node& operator=(const Node& other);
      Node& operator=(Node&& other) noexcept;

      // pure virtual render (Node � astratta)
      virtual void render() = 0;

      glm::mat4 getMatrix() const;
      void setMatrix(const glm::mat4& matrix);

      std::shared_ptr<Node>& getParent();
      void setParent(std::shared_ptr<Node> parent);

      std::vector<std::shared_ptr<Node>>& getChildren();
      void addChildren(std::shared_ptr<Node> child);
      std::shared_ptr<Node> removeChildren(const std::string& name);
      std::shared_ptr<Node> returnChild(const std::string& name);
      glm::mat4 getWorldMatrix() const;
   };

} // namespace Eng
