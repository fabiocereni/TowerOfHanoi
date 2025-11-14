#pragma once
#include "camera.h"

namespace Eng {
    class ENG_API Orthographic_Camera final : public Camera {

    public:
        Orthographic_Camera() noexcept;
        Orthographic_Camera(float left, float right,
                            float top, float bottom,
                            float nearPlane, float farPlane) noexcept;
        virtual ~Orthographic_Camera() noexcept;

        Orthographic_Camera(const Orthographic_Camera& other);
        Orthographic_Camera(Orthographic_Camera&& other) noexcept;
        Orthographic_Camera& operator=(const Orthographic_Camera& other);
        Orthographic_Camera& operator=(Orthographic_Camera&& other) noexcept;

        void render() override;

        float getLeft() const noexcept;
        float getRight() const noexcept;
        float getTop() const noexcept;
        float getBottom() const noexcept;
        float getFarPlane() const noexcept;
        float getNearPlane() const noexcept;

        void setLeft(float value) noexcept;
        void setRight(float value) noexcept;
        void setTop(float value) noexcept;
        void setBottom(float value) noexcept;
        void setFarPlane(float value) noexcept;
        void setNearPlane(float value) noexcept;

    protected:
        float left_;
        float right_;
        float top_;
        float bottom_;
        float nearPlane_;
        float farPlane_;
    };
}
