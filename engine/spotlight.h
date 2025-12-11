#pragma once
#include "light.h"

namespace Eng {

   /**
    * @class Spotlight
    * @brief Rappresenta una luce di tipo faretto (spotlight)
    */
   class ENG_API Spotlight final : public Light {
   public:

      /// @brief Distruttore della classe
      ~Spotlight() override;

      /// @brief Renderizza la luce nella scena
      void render(const glm::mat4& modelViewMatrix) override;


      /// @brief Imposta l'angolo di cutoff
      void setCutoff   (const float c) noexcept { cutoff_ = c; }
      /// @brief Imposta l'esponente di attenuazione
      void setExponent (const float e) noexcept { exponent_ = e; }
      /// @brief Imposta la direzione della luce
      void setDirection(const glm::vec3& d) noexcept { direction_ = d; }


      /// @brief Restituisce l'angolo di cutoff
      [[nodiscard]] float getCutoff()   const noexcept { return cutoff_; }
      /// @brief Restituisce l'esponente di attenuazione
      [[nodiscard]] float getExponent() const noexcept { return exponent_; }
      /// @brief Restituisce la direzione della luce
      [[nodiscard]] glm::vec3 getDirection() const noexcept { return direction_; }


      /// @brief Metodo statico per la creazione di una Spotlight
      static std::shared_ptr<Spotlight> createSpotLight();


   private:

      /// @brief Costruttore privato
      explicit Spotlight(int index) noexcept;


      /// @brief Direzione della luce
      glm::vec3 direction_{0,0,-1};
      /// @brief Angolo di cutoff
      float cutoff_   = 180.0f;
      /// @brief Esponente di attenuazione
      float exponent_ = 0.0f;
   };
}