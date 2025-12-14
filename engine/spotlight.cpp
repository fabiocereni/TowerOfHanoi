#include "spotlight.h"
#include <GL/freeglut.h>

#include "light_number_exception.h"
#include "glm/gtc/type_ptr.hpp"

namespace Eng {

   /**
    * @brief Costruttore della classe Spotlight
    * @param index Indice della luce OpenGL
    */
   Spotlight::Spotlight(const int index) noexcept
   : Light(index) {}


   /**
    * @brief Metodo statico per la creazione di una Spotlight
    * @return std::shared_ptr<Spotlight> Puntatore alla nuova luce creata
    * @throws LightNumberExceededException Se il numero massimo di luci è stato raggiunto
    */
   std::shared_ptr<Spotlight> Spotlight::createSpotLight() {
      if (lightNumber_ < 8) {
         return std::shared_ptr<Spotlight>(new Spotlight(lightNumber_++));
      }
      throw LightNumberExceededException("Maximum number of lights exceeded: (8)");
   }


   /**
    * @brief Distruttore
    * @details Disabilita la luce OpenGL e decrementa il contatore globale
    */
   Spotlight::~Spotlight() {
      const GLenum id = GL_LIGHT0 + index_;
      glDisable(id);
      lightNumber_--;
   }



   /**
    * @brief Renderizza la luce spotlight nella scena
    * @param modelViewMatrix Matrice ModelView corrente
    */
   void Spotlight::render(const glm::mat4& modelViewMatrix) {

         const GLenum id = GL_LIGHT0 + index_;

      // @details Controllo preventivo: se la luce è disattivata logicamente, la spegniamo in OpenGL e usciamo
      if (!isActive()) {
         glDisable(id);
         return;
      }

      // @brief Se siamo qui, la luce è attiva: abilitiamo
      glEnable(id);

      const auto ambient = glm::vec4(ambient_, 1.0f);
      const auto diffuse = glm::vec4(diffuse_, 1.0f);
      const auto specular = glm::vec4(specular_, 1.0f);
      const auto direction = glm::vec4(direction_, 0.0f);
      const auto position = glm::vec4(direction_, 1.0f);

      const float linearAttenuation = 4.5f / (radius_ > 0.0f ? radius_ : 1.0f);

      // @brief Impostazione dei parametri della luce
      glLightfv(id, GL_AMBIENT,  glm::value_ptr(ambient));
      glLightfv(id, GL_DIFFUSE,  glm::value_ptr(diffuse));
      glLightfv(id, GL_SPECULAR, glm::value_ptr(specular));
      glLightfv(id, GL_POSITION, glm::value_ptr(position));
      glLightfv(id, GL_SPOT_DIRECTION, glm::value_ptr(direction));
      glLightf(id, GL_SPOT_CUTOFF, cutoff_);
      glLightf(id, GL_SPOT_EXPONENT, exponent_);
      glLightf(id, GL_LINEAR_ATTENUATION,  linearAttenuation);

   }

}