#pragma once
#include <glm/glm.hpp>
#include "node.h"

enum class attenuation_mode_enum {
   NONE,
   LINEAR,
   QUADRATIC,
};

namespace Eng {
   class ENG_API OmnidirectionalLight {

   public:
      OmnidirectionalLight() noexcept;
      OmnidirectionalLight(const std::shared_ptr<Node>& node,
               const glm::mat4& worldMatrix,
               const glm::vec4& position,
               attenuation_mode_enum attenuationMode,
               float attenuationValue);

      virtual ~OmnidirectionalLight() noexcept;

      OmnidirectionalLight(const OmnidirectionalLight& other);
      OmnidirectionalLight(OmnidirectionalLight&& other) noexcept;

      OmnidirectionalLight& operator=(const OmnidirectionalLight& other);
      OmnidirectionalLight& operator=(OmnidirectionalLight&& other) noexcept;

      std::shared_ptr<Node> getNodePtr() const noexcept;
      glm::mat4 getNodeWorldMatrix() const noexcept;
      glm::vec4 getPosition() const noexcept;
      attenuation_mode_enum getAttenuationMode() const noexcept;
      float getAttenuationValue() const noexcept;

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
