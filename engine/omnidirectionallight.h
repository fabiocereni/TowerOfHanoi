#pragma once
#include <glm/glm.hpp>

#include "light.h"
#include "node.h"

enum class attenuation_mode_enum {
   NONE,
   LINEAR,
   QUADRATIC,
};

namespace Eng {
   class ENG_API OmnidirectionalLight final : public Light {

   public:
      OmnidirectionalLight() noexcept = default;
      OmnidirectionalLight(const std::shared_ptr<Node>& node,
               const glm::mat4& worldMatrix,
               const glm::vec4& position,
               attenuation_mode_enum attenuationMode,
               float attenuationValue);

      ~OmnidirectionalLight() noexcept override = default;

      OmnidirectionalLight(const OmnidirectionalLight& other) = default;
      OmnidirectionalLight(OmnidirectionalLight&& other) noexcept = default;

      OmnidirectionalLight& operator=(const OmnidirectionalLight& other) = default;
      OmnidirectionalLight& operator=(OmnidirectionalLight&& other) noexcept = default;

      void render(const glm::mat4 &C) override;

      [[nodiscard]] std::shared_ptr<Node> getNodePtr() const noexcept;
      [[nodiscard]] glm::mat4 getNodeWorldMatrix() const noexcept;
      [[nodiscard]] glm::vec4 getPosition() const noexcept;
      [[nodiscard]] attenuation_mode_enum getAttenuationMode() const noexcept;
      [[nodiscard]] float getAttenuationValue() const noexcept;

      void setNodePtr(const std::shared_ptr<Node>& node) noexcept;
      void setNodeWorldMatrix(const glm::mat4& worldMatrix) noexcept;
      void setPosition(const glm::vec4& position) noexcept;
      void setAttenuationMode(attenuation_mode_enum mode) noexcept;
      void setAttenuationValue(float value) noexcept;

   protected:
      std::shared_ptr<Node> node_ptr_;
      glm::mat4 nodeWorldMatrix_;
      glm::vec4 position_;
      attenuation_mode_enum attenuation_mode_;
      float attenuation_value_;
   };
}
