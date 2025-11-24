#pragma once
#include <glm/mat4x4.hpp>
#include "node.h"

namespace Eng {

   class ENG_API Camera : public Node {
   public:
      Camera() noexcept;
      ~Camera() noexcept override = default;
      Camera(const Camera& other) = default;
      Camera(Camera&& other) noexcept = default;
      Camera& operator=(const Camera& other) = default;
      Camera& operator=(Camera&& other) noexcept = default;

      void render(const glm::mat4 &C) override = 0;

      void setProjectionMatrix(const glm::mat4& matrix) noexcept;
      [[nodiscard]] glm::mat4 getProjectionMatrix() const noexcept;
      [[nodiscard]] glm::mat4 computeCameraInverse() const noexcept;

   protected:
      glm::mat4 projectionMatrix_;
   };
}
