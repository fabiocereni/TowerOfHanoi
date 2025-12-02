#pragma once
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Eng {
    class ENG_API Perspective_Camera final : public Camera {

    public:
        Perspective_Camera(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;

        ~Perspective_Camera() noexcept = default;

        Perspective_Camera(const Perspective_Camera& other) = delete;
        Perspective_Camera(Perspective_Camera&& other) noexcept = default;
        Perspective_Camera& operator=(const Perspective_Camera& other) = delete;
        Perspective_Camera& operator=(Perspective_Camera&& other) noexcept = default;

        void setFov(const float fov) noexcept {fov_ = fov; updateProjection();};
        void setAspectRatio(const float aspectRatio) noexcept {aspectRatio_ = aspectRatio; updateProjection();};
        void setNearPlane(const float nearPlane) noexcept {nearPlane_ = nearPlane; updateProjection();};
        void setFarPlane(const float farPlane) noexcept {farPlane_ = farPlane; updateProjection();};

        [[nodiscard]] float getFov() const noexcept {return fov_;};
        [[nodiscard]] float getAspectRatio() const noexcept {return aspectRatio_;};
        [[nodiscard]] float getNearPlane() const noexcept {return nearPlane_;};
        [[nodiscard]] float getFarPlane() const noexcept {return farPlane_;};


        [[nodiscard]] glm::mat4 getProjectionMatrix() const noexcept override {return projectionMatrix_;};

        // serve per aggiornare la matrice in caso di cambiamenti dei valori
        void updateProjection() noexcept;

        void render(const glm::mat4& modelViewMatrix) override;
        void onResize(int width, int height) noexcept override;


    protected:
        float fov_;
        float aspectRatio_;
        float nearPlane_;
        float farPlane_;

        glm::mat4 projectionMatrix_ = {1.0};

    };
}
