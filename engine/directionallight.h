#pragma once
#include "light.h"

namespace Eng {
   /**
    * @class DirectionalLight
    */
   class ENG_API DirectionalLight final : public Light {
   public:

   /// @brief Distruttore
   ~DirectionalLight() override;

   /// @brief Render della luce direzionale
   void render(const glm::mat4& modelViewMatrix) override;


   /// @brief Factory method per crare una luce
   static std::shared_ptr<DirectionalLight> createDirectionalLight();

   /// @brief getters e setters
   void setDirection(const glm::vec3& d) noexcept { direction_ = d; }
   [[nodiscard]] glm::vec3 getDirection() const noexcept { return direction_; }



   private:
      /// @brief Costruttore privato per Factory Method
      explicit DirectionalLight(int index) noexcept;

      /// @brief Direzione della luce
      glm::vec3 direction_{0,0,-1};
   };
}
