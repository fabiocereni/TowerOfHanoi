#include "omnidirectionallight.h"
#include <GL/freeglut.h>

#include "light_number_exception.h"
#include "glm/gtc/type_ptr.hpp"

namespace Eng {

   /**
    * @brief Costruttore della luce omnidirezionale
    * @param index Indice assegnato alla luce
    */
   OmnidirectionalLight::OmnidirectionalLight(const int index) noexcept
   : Light(index) {}


   /**
    * @brief Crea una nuova istanza di luce omnidirezionale
    * @return std::shared_ptr<OmnidirectionalLight> Puntatore alla nuova luce
    * @throws LightNumberExceededException Se si supera il numero massimo di luci consentite
    */
   std::shared_ptr<OmnidirectionalLight> OmnidirectionalLight::createOmnidirectionalLight() {

      // @details Controllo se è stato raggiunto il limite massimo di luci OpenGL (8)
      if (lightNumber_ < 8) {
         return std::shared_ptr<OmnidirectionalLight>(new OmnidirectionalLight(lightNumber_++));
      }
      throw LightNumberExceededException("Maximum number of lights exceeded: (8)");
   }

   /**
    * @brief Distruttore della classe
    * @details Disabilita la luce OpenGL associata e decrementa il contatore globale delle luci
    */
   OmnidirectionalLight::~OmnidirectionalLight() {
      const GLenum id = GL_LIGHT0 + index_;
      glDisable(id);
      lightNumber_--;
   }


   /**
    * @brief Applica i parametri della luce allo stato OpenGL
    * @param modelViewMatrix Matrice ModelView corrente
    */
   void OmnidirectionalLight::render(const glm::mat4& modelViewMatrix) {


      const GLenum id = GL_LIGHT0 + index_;

      // @details Controllo preventivo: se la luce è disattivata logicamente, la spegniamo in OpenGL e usciamo
      if (!isActive()) {
         glDisable(id);
         return;
      }

      // @brief Se siamo qui, la luce è attiva: abilitiamo la sorgente luminosa
      glEnable(id);

      const auto ambient = glm::vec4(ambient_, 1.0f);
      const auto diffuse = glm::vec4(diffuse_, 1.0f);
      const auto specular = glm::vec4(specular_, 1.0f);

      // @brief Calcolo dell'attenuazione lineare in base al raggio configurato
      const float linearAttenuation = 4.5f / (radius_ > 0.0f ? radius_ : 1.0f);


      // @brief Calcolo della posizione della luce nello spazio vista
      const glm::vec4 posEye = modelViewMatrix * lightPosition_;



      // @brief Invio dei parametri calcolati al contesto OpenGL
      glLightfv(id, GL_AMBIENT,  glm::value_ptr(ambient));
      glLightfv(id, GL_DIFFUSE,  glm::value_ptr(diffuse));
      glLightfv(id, GL_SPECULAR, glm::value_ptr(specular));
      glLightfv(id, GL_POSITION, glm::value_ptr(posEye));
      glLightf(id, GL_LINEAR_ATTENUATION, linearAttenuation);

   }
}