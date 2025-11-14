#pragma once
#include <glm/mat4x4.hpp>
#include "node.h"

namespace Eng {

   class ENG_API Camera : public Node {
   public:
      Camera() noexcept;
      virtual ~Camera() noexcept;
      Camera(const Camera& other);
      Camera(Camera&& other) noexcept;
      Camera& operator=(const Camera& other);
      Camera& operator=(Camera&& other) noexcept;

      virtual void render() = 0;

      void setProjectionMatrix(const glm::mat4& matrix) noexcept;
      glm::mat4 getProjectionMatrix() const noexcept;
      glm::mat4 computeCameraInverse() const noexcept;

   protected:
      glm::mat4 projectionMatrix_;
   };
}
