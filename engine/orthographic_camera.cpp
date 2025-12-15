#include "orthographic_camera.h"

using namespace Eng;

/**
 * @brief Costruttore della camera ortografica
 * @param left Coordinata del piano sinistro
 * @param right Coordinata del piano destro
 * @param top Coordinata del piano superiore
 * @param bottom Coordinata del piano inferiore
 * @param nearPlane Distanza del piano vicino
 * @param farPlane Distanza del piano lontano
 */
Orthographic_Camera::Orthographic_Camera(const float left, const float right,
                                         const float top, const float bottom,
                                         const float nearPlane, const float farPlane) noexcept
    : left_{left}, right_{right}, top_{top}, bottom_{bottom}, nearPlane_{nearPlane}, farPlane_{farPlane}
{
    // @brief Calcola la matrice di proiezione ortografica iniziale
    projectionMatrix_ = glm::ortho(left_, right_, bottom_, top_, nearPlane_, farPlane_);
}

/**
 * @brief Metodo di rendering
 * @param modelViewMatrix Matrice ModelView corrente
 * @details La camera non ha geometria visibile da renderizzare, quindi il metodo è vuoto
 */
void Orthographic_Camera::render(const glm::mat4& modelViewMatrix) {}


/**
 * @brief Gestisce il ridimensionamento della finestra
 * @param width Nuova larghezza della viewport
 * @param height Nuova altezza della viewport
 * @details Aggiorniamo le dimensioni interne della camera
 *          per riflettere le nuove dimensioni della viewport
 */
void Orthographic_Camera::onResize(const int width, const int height) noexcept {
    left_ = 0.0f;
    bottom_ = 0.0f;
    right_ = static_cast<float>(width);
    top_ = static_cast<float>(height);
    projectionMatrix_ = glm::ortho(left_, right_, bottom_, top_, nearPlane_, farPlane_);
}