#include "directionallight.h"
#include "light_number_exception.h"
#include <GL/freeglut.h>

#include "glm/gtc/type_ptr.hpp"

namespace Eng {

    /**
     * @brief Costruttore
     * @param index
     * @return Crea e torna una luce direzionale
     */
    DirectionalLight::DirectionalLight(const int index) noexcept
        : Light(index) {}


    /**
     * @brief Factory Method per creare una luce direzionale
     * @details Se le luci create sono meno di 8 viene
     * creata una nuova luce, altrimenti viene lanciata un'eccezione
     * @return Crea e torna una luce direzionale
     */
    std::shared_ptr<DirectionalLight> DirectionalLight::createDirectionalLight() {

        if (lightNumber_ < 8) {
            return std::shared_ptr<DirectionalLight>(new DirectionalLight(lightNumber_++));
        }
        throw LightNumberExceededException("Maximum number of lights exceeded: (8)");
    }


    DirectionalLight::~DirectionalLight() noexcept {
        const GLenum id = GL_LIGHT0 + index_;
        glDisable(id);
        lightNumber_--;
    }


    /**
     * @brief Metodo che si occupa di renderizzare la luce
     * @param modelViewMatrix
     */
    void DirectionalLight::render(const glm::mat4& modelViewMatrix) {

        const GLenum id = GL_LIGHT0 + index_;
        glEnable(id);


        const auto ambient = glm::vec4(ambient_, 1.0f);
        const auto diffuse = glm::vec4(diffuse_, 1.0f);
        const auto specular = glm::vec4(specular_, 1.0f);

        const auto position = glm::vec4(direction_, 0.0f);


        glLightfv(id, GL_AMBIENT,  glm::value_ptr(ambient));
        glLightfv(id, GL_DIFFUSE,  glm::value_ptr(diffuse));
        glLightfv(id, GL_SPECULAR, glm::value_ptr(specular));
        glLightfv(id, GL_POSITION, glm::value_ptr(position));

    }

}
