#pragma once
#include "node.h"
#include <glm/glm.hpp>

namespace Eng {
   class ENG_API Light : public Node {
   public:
      Light() noexcept;
      Light(bool smoothShading,
            int lightNumber,
            const glm::vec4& ambient,
            const glm::vec4& diffuse,
            const glm::vec4& specular,
            bool ambientEnabled,
            bool localViewerEnabled,
            const glm::vec4& position);

      virtual ~Light() noexcept;

      Light(const Light& other);
      Light(Light&& other) noexcept;

      Light& operator=(const Light& other);
      Light& operator=(Light&& other) noexcept;

      bool getSmoothShading() const noexcept;
      int getLightNumber() const noexcept;
      glm::vec4 getAmbient() const noexcept;
      glm::vec4 getDiffuse() const noexcept;
      glm::vec4 getSpecular() const noexcept;
      bool isAmbientEnabled() const noexcept;
      bool isLocalViewer() const noexcept;
      glm::vec4 getPosition() const noexcept;

      void setSmoothShading(bool smoothShading) noexcept;
      void setLightNumber(int lightNumber) noexcept;
      void setAmbient(const glm::vec4& ambient) noexcept;
      void setDiffuse(const glm::vec4& diffuse) noexcept;
      void setSpecular(const glm::vec4& specular) noexcept;
      void setLocalViewer(bool enabled) noexcept;
      void setAmbientEnabled(bool enabled) noexcept;
      void setPosition(const glm::vec4& position) noexcept;

   protected:
      bool smoothShading_;
      int lightNumber_;
      glm::vec4 ambient_;
      glm::vec4 diffuse_;
      glm::vec4 specular_;
      bool ambientEnabled_;
      bool localViewerEnabled_;
      glm::vec4 position_;
   };
}
