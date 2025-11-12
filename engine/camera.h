#pragma once
#include <glm/mat4x4.hpp>
#include "node.h"

namespace Eng {

   class ENG_API Camera : public Node {
   protected:
      glm::mat4 projectionMatrix;

   public:
      Camera();
      virtual ~Camera();
      Camera(const Camera& other);
      Camera(Camera&& other) noexcept;
      Camera& operator=(const Camera& other);
      Camera& operator=(Camera&& other) noexcept;

      virtual void render() override;

      void setProjectionMatrix(const glm::mat4& matrix);
      glm::mat4 getProjectionMatrix() const;
      glm::mat4 computeCameraInverse() const;
   };

}