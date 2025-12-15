#include "perspective_camera.h"

using namespace Eng;

/**
 * @brief Costruttore della camera prospettica
 * @param fov Campo visivo verticale in gradi
 * @param aspectRatio Rapporto d'aspetto (larghezza/altezza)
 * @param nearPlane Distanza del piano vicino (near clipping plane)
 * @param farPlane Distanza del piano lontano (far clipping plane)
 */
Perspective_Camera::Perspective_Camera(const float fov,
                                       const float aspectRatio,
                                       const float nearPlane,
                                       const float farPlane) noexcept
    : fov_{fov}, aspectRatio_{aspectRatio}, nearPlane_{nearPlane}, farPlane_{farPlane}
{
    updateProjection();
}

/**
 * @brief Ricalcola la matrice di proiezione prospettica
 * @details Utilizza glm::perspective con i parametri correnti della camera
 */
void Perspective_Camera::updateProjection() noexcept {
    projectionMatrix_ = glm::perspective(glm::radians(fov_), aspectRatio_, nearPlane_, farPlane_);
}


/**
 * @brief Metodo di rendering
 * @param modelViewMatrix Matrice ModelView corrente
 * @details La camera non ha geometria visibile, il metodo è vuoto
 */
void Perspective_Camera::render(const glm::mat4& modelViewMatrix) {}

/**
 * @brief Gestisce il ridimensionamento della finestra
 * @param width Nuova larghezza della viewport
 * @param height Nuova altezza della viewport
 */
void Perspective_Camera::onResize(const int width, const int height) noexcept {
    // @brief Unico valore che cambia
    setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
    // @brief Aggiorno con i nuovi valori
    updateProjection();
}