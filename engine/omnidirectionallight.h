#pragma once
#include "light.h"

namespace Eng {

   /**
    * @class OmnidirectionalLight
    * @brief Rappresenta una luce omnidirezionale
    */
   class ENG_API OmnidirectionalLight final : public Light {
   public:

      /// @brief Distruttore della classe
      ~OmnidirectionalLight() override;

      /// @brief Renderizza la luce nella scena
      void render(const glm::mat4& modelViewMatrix) override;


      /// @brief Metodo statico per la creazione di una luce omnidirezionale
      static std::shared_ptr<OmnidirectionalLight> createOmnidirectionalLight();

      /// @brief Restituisce la posizione della luce
      glm::vec4 getPosition() { return lightPosition_; }

      /// @brief Imposta la posizione della luce
      void setPosition(const glm::vec4 position) { lightPosition_ = position; }


   private:

      /// @brief Costruttore privato
      explicit OmnidirectionalLight(int index) noexcept;

      /// @brief Posizione della luce
      glm::vec4 lightPosition_ = glm::vec4(1.0f);
   };

}