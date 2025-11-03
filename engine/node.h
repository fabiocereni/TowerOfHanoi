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
      std::unique_ptr<Node> parent;
      std::vector<std::unique_ptr<Node>> children;

   public:
      Node();
      virtual ~Node();
      Node(const Node& other);
      Node(Node&& other) noexcept;
      Node& operator=(const Node& other);
      Node& operator=(Node&& other) noexcept;

      // pure virtual render (Node è astratta)
      virtual void render() = 0;

      glm::mat4 getMatrix() const;
      void setMatrix(const glm::mat4& matrix);

      std::unique_ptr<Node>& getParent();
      void setParent(std::unique_ptr<Node> parent);

      std::vector<std::unique_ptr<Node>>& getChildren();
      void addChildren(std::unique_ptr<Node> child);
      std::unique_ptr<Node> removeChildren(const std::string& name);
      std::unique_ptr<Node> returnChild(const std::string& name);
      glm::mat4 getWorldMatrix() const;
   };

} // namespace Eng