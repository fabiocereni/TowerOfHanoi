#pragma once
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Eng {
    /**
     * @class Perspective_Camera
     * @brief Rappresenta una camera con proiezione prospettica
     */
    class ENG_API Perspective_Camera final : public Camera {

    public:
        /**
         * @brief Costruttore della camera prospettica
         * @param fov Campo visivo verticale (Field of View) in gradi
         * @param aspectRatio Rapporto d'aspetto (larghezza/altezza)
         * @param nearPlane Distanza del piano vicino
         * @param farPlane Distanza del piano lontano
         */
        Perspective_Camera(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;

        /// @brief Distruttore di default
        ~Perspective_Camera() noexcept = default;

        /// @brief Costruttori di copia e movimento e operatori di assegnamento
        Perspective_Camera(const Perspective_Camera& other) = default;
        Perspective_Camera(Perspective_Camera&& other) noexcept = default;
        Perspective_Camera& operator=(const Perspective_Camera& other) = default;
        Perspective_Camera& operator=(Perspective_Camera&& other) noexcept = default;

        /// @brief Imposta il FOV e aggiorna la matrice di proiezione
        void setFov(const float fov) noexcept {fov_ = fov; updateProjection();};
        /// @brief Imposta l'aspect ratio e aggiorna la matrice di proiezione
        void setAspectRatio(const float aspectRatio) noexcept {aspectRatio_ = aspectRatio; updateProjection();};
        /// @brief Imposta il piano near e aggiorna la matrice di proiezione
        void setNearPlane(const float nearPlane) noexcept {nearPlane_ = nearPlane; updateProjection();};
        /// @brief Imposta il piano far e aggiorna la matrice di proiezione
        void setFarPlane(const float farPlane) noexcept {farPlane_ = farPlane; updateProjection();};

        /// @brief Restituisce il valore del FOV
        [[nodiscard]] float getFov() const noexcept {return fov_;};
        /// @brief Restituisce il valore dell'aspect ratio
        [[nodiscard]] float getAspectRatio() const noexcept {return aspectRatio_;};
        /// @brief Restituisce la distanza del piano near
        [[nodiscard]] float getNearPlane() const noexcept {return nearPlane_;};
        /// @brief Restituisce la distanza del piano far
        [[nodiscard]] float getFarPlane() const noexcept {return farPlane_;};


        /// @brief Restituisce la matrice di proiezione calcolata
        [[nodiscard]] glm::mat4 getProjectionMatrix() const noexcept override {return projectionMatrix_;};

        // serve per aggiornare la matrice in caso di cambiamenti dei valori
        /// @brief Aggiorna la matrice di proiezione utilizzando i parametri correnti
        void updateProjection() noexcept;

        /// @brief Renderizza la camera (override)
        void render(const glm::mat4& modelViewMatrix) override;

        /// @brief Gestisce il ridimensionamento della finestra ricalcolando l'aspect ratio
        void onResize(int width, int height) noexcept override;


    protected:
        /// @brief Campo visivo verticale in gradi
        float fov_;
        /// @brief Rapporto d'aspetto
        float aspectRatio_;
        /// @brief Distanza del piano vicino
        float nearPlane_;
        /// @brief Distanza del piano lontano
        float farPlane_;
        /// @brief Matrice di proiezione
        glm::mat4 projectionMatrix_ = {1.0};
    };
}