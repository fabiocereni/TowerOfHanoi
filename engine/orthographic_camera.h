#pragma once
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Eng {
    class ENG_API Orthographic_Camera final : public Camera {

    public:
        Orthographic_Camera() noexcept = default;
        Orthographic_Camera(float left,float right,float top, float bottom,float nearPlane,float farPlane) noexcept;

        virtual ~Orthographic_Camera() noexcept = default;

        Orthographic_Camera(const Orthographic_Camera& other) = default;
        Orthographic_Camera(Orthographic_Camera&& other) noexcept = default;
        Orthographic_Camera& operator=(const Orthographic_Camera& other) = default;
        Orthographic_Camera& operator=(Orthographic_Camera&& other) noexcept = default;

        void render(const glm::mat4 &C, glm::mat4 M = glm::mat4(1.0f)) override;

        [[nodiscard]] float getLeft() const noexcept;
        [[nodiscard]] float getRight() const noexcept;
        [[nodiscard]] float getTop() const noexcept;
        [[nodiscard]] float getBottom() const noexcept;
        [[nodiscard]] float getFarPlane() const noexcept;
        [[nodiscard]] float getNearPlane() const noexcept;



        void setLeft(float left) noexcept;
        void setRight(float right) noexcept;
        void setTop(float top) noexcept;
        void setBottom(float bottom) noexcept;
        void setFarPlane(float farPlane) noexcept;
        void setNearPlane(float nearPlane) noexcept;

    protected:
        float left_;
        float right_;
        float top_;
        float bottom_;
        float nearPlane_;
        float farPlane_;
    };
}
