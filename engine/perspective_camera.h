#pragma once
#include "camera.h"

namespace Eng {
    class ENG_API Perspective_Camera final : public Camera {

    public:
        Perspective_Camera() noexcept = default;
        Perspective_Camera(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;
        virtual ~Perspective_Camera() noexcept = default;

        Perspective_Camera(const Perspective_Camera& other) = default;
        Perspective_Camera(Perspective_Camera&& other) noexcept = default;
        Perspective_Camera& operator=(const Perspective_Camera& other) = default;
        Perspective_Camera& operator=(Perspective_Camera&& other) noexcept = default;

        void render() override;

        void setFov(float fov) noexcept;
        void setAspectRatio(float aspectRatio) noexcept;
        void setNearPlane(float nearPlane) noexcept;
        void setFarPlane(float farPlane) noexcept;

        [[nodiscard]] float getFov() const noexcept;
        [[nodiscard]] float getAspectRatio() const noexcept;
        [[nodiscard]] float getNearPlane() const noexcept;
        [[nodiscard]] float getFarPlane() const noexcept;

    protected:
        float fov_;
        float aspectRatio_;
        float nearPlane_;
        float farPlane_;
    };
}
