#pragma once
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Eng {
    /**
     * @class Orthographic_Camera
     * @brief Rappresenta una camera con proiezione ortografica
     */
    class ENG_API Orthographic_Camera final : public Camera {

    public:
        /// @brief Costruttore di default
        Orthographic_Camera() noexcept = default;

        /**
         * @brief Costruttore con parametri di proiezione
         * @param left Coordinata del piano sinistro
         * @param right Coordinata del piano destro
         * @param top Coordinata del piano superiore
         * @param bottom Coordinata del piano inferiore
         * @param nearPlane Distanza del piano vicino
         * @param farPlane Distanza del piano lontano
         */
        Orthographic_Camera(float left,float right,float top, float bottom,float nearPlane,float farPlane) noexcept;

        /// @brief Distruttore
        ~Orthographic_Camera() noexcept = default;

        /// @brief Costruttori di copia e movimento e operatori di assegnamento
        Orthographic_Camera(const Orthographic_Camera& other) = default;
        Orthographic_Camera(Orthographic_Camera&& other) noexcept = default;
        Orthographic_Camera& operator=(const Orthographic_Camera& other) = default;
        Orthographic_Camera& operator=(Orthographic_Camera&& other) noexcept = default;


        /// @brief Restituisce la coordinata sinistra del frustum
        [[nodiscard]] float getLeft() const noexcept {return left_;};
        /// @brief Restituisce la coordinata destra del frustum
        [[nodiscard]] float getRight() const noexcept {return right_;};
        /// @brief Restituisce la coordinata superiore del frustum
        [[nodiscard]] float getTop() const noexcept {return top_;};
        /// @brief Restituisce la coordinata inferiore del frustum
        [[nodiscard]] float getBottom() const noexcept {return bottom_;};
        /// @brief Restituisce la distanza del piano lontano
        [[nodiscard]] float getFarPlane() const noexcept {return farPlane_;};
        /// @brief Restituisce la distanza del piano vicino
        [[nodiscard]] float getNearPlane() const noexcept {return nearPlane_;};


        /// @brief Imposta la coordinata sinistra del frustum
        void setLeft(const float left) noexcept {left_ = left;};
        /// @brief Imposta la coordinata destra del frustum
        void setRight(const float right) noexcept {right_ = right;};
        /// @brief Imposta la coordinata superiore del frustum
        void setTop(const float top) noexcept {top_ = top;};
        /// @brief Imposta la coordinata inferiore del frustum
        void setBottom(const float bottom) noexcept {bottom_ = bottom;};
        /// @brief Imposta la distanza del piano lontano
        void setFarPlane(const float farPlane) noexcept {farPlane_ = farPlane;};
        /// @brief Imposta la distanza del piano vicino
        void setNearPlane(const float nearPlane) noexcept {nearPlane_ = nearPlane;};

        /// @brief Restituisce la matrice di proiezione calcolata
        [[nodiscard]] glm::mat4 getProjectionMatrix() const noexcept override {return projectionMatrix_;};

        /// @brief Renderizza la camera (override)
        void render(const glm::mat4& modelViewMatrix) override;

        /// @brief Gestisce il ridimensionamento della finestra ricalcolando la proiezione
        void onResize(int width, int height) noexcept override;


    protected:
        /// @brief Coordinata sinistra del frustum
        float left_;
        /// @brief Coordinata destra del frustum
        float right_;
        /// @brief Coordinata superiore del frustum
        float top_;
        /// @brief Coordinata inferiore del frustum
        float bottom_;
        /// @brief Distanza del piano vicino
        float nearPlane_;
        /// @brief Distanza del piano lontano
        float farPlane_;

        /// @brief Matrice di proiezione
        glm::mat4 projectionMatrix_= {1.0};
    };
}