#pragma once
#include <glm/glm.hpp>


#include "light.h"

namespace Eng {
   class ENG_API DirectionalLight final : public Light {

   public:
      DirectionalLight() noexcept = default;
      DirectionalLight(const glm::vec3& color,
                 const glm::vec3& direction) noexcept;


      DirectionalLight(const DirectionalLight& other) = default;
      DirectionalLight(DirectionalLight&& other) noexcept = default;

      DirectionalLight& operator=(const DirectionalLight& other) = default;
      DirectionalLight& operator=(DirectionalLight&& other) noexcept = default;

      ~DirectionalLight() noexcept override = default;

      void render(const glm::mat4& modelViewMatrix) override;

      [[nodiscard]] glm::mat4 getNodeWorldMatrix() const noexcept;
      [[nodiscard]] glm::vec3 getDirection() const noexcept {return direction_;};
      [[nodiscard]] glm::vec4 getPosition() const noexcept {return position_;};

      void setNodeWorldMatrix(const glm::mat4& worldMatrix) noexcept;
      void setDirection(const glm::vec4& direction) noexcept {direction_ = direction;};
      void setPosition(glm::vec4 position) noexcept {position_ = position;};

   protected:

      glm::mat4 nodeWorldMatrix_;
      glm::vec3 color_;
      glm::vec3 direction_;
      glm::vec4 position_;
   };
}
