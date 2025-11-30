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

      void render(const glm::mat4 &C, glm::mat4 M = glm::mat4(1.0f)) override;

      [[nodiscard]] bool getSmoothShading() const noexcept;
      [[nodiscard]] int getLightNumber() const noexcept;
      [[nodiscard]] glm::vec4 getAmbient() const noexcept;
      [[nodiscard]] glm::vec4 getDiffuse() const noexcept;
      [[nodiscard]] glm::vec4 getSpecular() const noexcept;
      [[nodiscard]] bool isAmbientEnabled() const noexcept;
      [[nodiscard]] bool isLocalViewer() const noexcept;

      void setSmoothShading(bool smoothShading) noexcept;
      void setLightNumber(int lightNumber) noexcept;
      void setAmbient(const glm::vec4& ambient) noexcept;
      void setDiffuse(const glm::vec4& diffuse) noexcept;
      void setSpecular(const glm::vec4& specular) noexcept;
      void setLocalViewer(bool enabled) noexcept;
      void setAmbientEnabled(bool enabled) noexcept;

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
