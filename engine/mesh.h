#pragma once
#include <vector>
#include <GL/freeglut.h>
#include "node.h"

namespace Eng {

   class ENG_API Mesh : public Node {
   protected:
      std::vector<GLfloat> vertexes;

   public:
      Mesh();
      virtual ~Mesh();
      Mesh(const Mesh& other);
      Mesh(Mesh&& other) noexcept;
      Mesh& operator=(const Mesh& other);
      Mesh& operator=(Mesh&& other) noexcept;

      virtual void render() override;
   };

}