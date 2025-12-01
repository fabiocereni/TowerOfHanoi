#pragma once
#include "node.h"
#include <glm/glm.hpp>

namespace Eng {
   class ENG_API Light : public Node {
   public:
      Light() noexcept = default;
      Light(bool smoothShading,
            int lightNumber,
            const glm::vec4& ambient,
            const glm::vec4& diffuse,
            const glm::vec4& specular,
            bool ambientEnabled,
            bool localViewerEnabled);

      virtual ~Light() noexcept = default;

      Light(const Light& other) = default;
      Light(Light&& other) noexcept = default;

      Light& operator=(const Light& other) = default;
      Light& operator=(Light&& other) noexcept = default;

      void render(const glm::mat4& modelViewMatrix) override;

      [[nodiscard]] bool getSmoothShading() const noexcept {return smoothShading_;};
      [[nodiscard]] int getLightNumber() const noexcept {return lightNumber_;};
      [[nodiscard]] glm::vec4 getAmbient() const noexcept {return ambient_;};
      [[nodiscard]] glm::vec4 getDiffuse() const noexcept {return diffuse_;};
      [[nodiscard]] glm::vec4 getSpecular() const noexcept {return specular_;};
      [[nodiscard]] bool isAmbientEnabled() const noexcept {return ambientEnabled_;};
      [[nodiscard]] bool isLocalViewer() const noexcept {return localViewerEnabled_;};

      void setSmoothShading(bool smoothShading) noexcept {smoothShading_ = smoothShading;};
      void setLightNumber(int lightNumber) noexcept {lightNumber_ = lightNumber;};
      void setAmbient(const glm::vec4& ambient) noexcept {ambient_ = ambient;};
      void setDiffuse(const glm::vec4& diffuse) noexcept {diffuse_ = diffuse;};
      void setSpecular(const glm::vec4& specular) noexcept {specular_ = specular;};
      void setLocalViewer(const bool enabled) noexcept {localViewerEnabled_ = enabled;};
      void setAmbientEnabled(const bool enabled) noexcept {ambientEnabled_ = enabled;};

   protected:
      bool smoothShading_;
      int lightNumber_;
      glm::vec4 ambient_;
      glm::vec4 diffuse_;
      glm::vec4 specular_;
      bool ambientEnabled_;
      bool localViewerEnabled_;
      static int number_of_lights;
   };
}
