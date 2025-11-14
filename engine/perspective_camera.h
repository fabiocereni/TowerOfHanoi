#pragma once
#include "camera.h"

namespace Eng {
    class ENG_API Perspective_Camera final : public Camera {

    public:
        Perspective_Camera() noexcept;
        Perspective_Camera(float fov, float aspectRatio,
                           float nearPlane, float farPlane) noexcept;
        virtual ~Perspective_Camera() noexcept;

        Perspective_Camera(const Perspective_Camera& other);
        Perspective_Camera(Perspective_Camera&& other) noexcept;
        Perspective_Camera& operator=(const Perspective_Camera& other);
        Perspective_Camera& operator=(Perspective_Camera&& other) noexcept;

        void render() override;

        void setFov(float fov) noexcept;
        void setAspectRatio(float aspectRatio) noexcept;
        void setNearPlane(float nearPlane) noexcept;
        void setFarPlane(float farPlane) noexcept;

        float getFov() const noexcept;
        float getAspectRatio() const noexcept;
        float getNearPlane() const noexcept;
        float getFarPlane() const noexcept;

    protected:
        float fov_;
        float aspectRatio_;
        float nearPlane_;
        float farPlane_;
    };
}
