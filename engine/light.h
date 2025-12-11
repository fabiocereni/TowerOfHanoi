#pragma once
#include "engine_api.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "node.h"

namespace Eng {
   /**
    * @class Light
    */
   class ENG_API Light : public Node {
   public:

      /// @brief Costruttore e distruttore
      explicit Light(int index) noexcept;
      virtual ~Light() noexcept = default;


      void render(const glm::mat4& modelViewMatrix) override = 0;



      /// @brief getter e setter
      void setAmbient(const glm::vec3& c) noexcept { ambient_ = c; }
      void setDiffuse(const glm::vec3& c) noexcept { diffuse_ = c; }
      void setSpecular(const glm::vec3& c) noexcept { specular_ = c; }
      void setRadius(const float r) noexcept { radius_ = r; }
      void setConstantAtt(const float c) noexcept { attConst_ = c; }
      void setLinearAtt(const float l) noexcept { attLinear_ = l; }
      void setQuadraticAtt(const float q) noexcept { attQuad_ = q; }
      void setActive(const bool newState) noexcept { active_ = newState; }


      [[nodiscard]] static int getLightNumber() noexcept { return lightNumber_; }


      [[nodiscard]] glm::vec3 getAmbient() const noexcept { return ambient_; }
      [[nodiscard]] glm::vec3 getDiffuse() const noexcept { return diffuse_; }
      [[nodiscard]] glm::vec3 getSpecular() const noexcept { return specular_; }
      [[nodiscard]] float getRadius() const noexcept { return radius_; }
      [[nodiscard]] float getConstantAtt() const noexcept { return attConst_; }
      [[nodiscard]] float getLinearAtt() const noexcept { return attLinear_; }
      [[nodiscard]] float getQuadraticAtt() const noexcept { return attQuad_; }
      [[nodiscard]] bool isActive() const noexcept { return active_; }

      void toggleLight() { active_ = !active_; }

   protected:

      /// @brief Stato attuale
      bool active_ = false;

      /// @brief Numero globale di luci
      static int lightNumber_;
      /// @brief Numero di ogni luce
      int index_;

      /// @brief Componente ambient
      glm::vec3 ambient_{0.0f};
      /// @brief Componente diffuse
      glm::vec3 diffuse_{1.0f};
      /// @brief Componente speculare
      glm::vec3 specular_{1.0f};

      /// @brief Distanza di contribuzione
      float radius_ = 1.0f;

      /// @brief Attenuazione costante
      float attConst_ = 1.0f;
      /// @brief Attenuazione lineare
      float attLinear_ = 0.0f;
      /// @brief Attenuazione quadratica
      float attQuad_ = 0.0f;
   };
}
