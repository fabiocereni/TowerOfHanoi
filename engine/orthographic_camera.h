#pragma once
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Eng {
    class ENG_API Orthographic_Camera final : public Camera {

    public:
        Orthographic_Camera() noexcept = default;
        Orthographic_Camera(float left,float right,float top, float bottom,float nearPlane,float farPlane) noexcept;

        ~Orthographic_Camera() noexcept = default;

        Orthographic_Camera(const Orthographic_Camera& other) = delete;
        Orthographic_Camera(Orthographic_Camera&& other) noexcept = default;
        Orthographic_Camera& operator=(const Orthographic_Camera& other) = delete;
        Orthographic_Camera& operator=(Orthographic_Camera&& other) noexcept = default;


        [[nodiscard]] float getLeft() const noexcept {return left_;};
        [[nodiscard]] float getRight() const noexcept {return right_;};
        [[nodiscard]] float getTop() const noexcept {return top_;};
        [[nodiscard]] float getBottom() const noexcept {return bottom_;};
        [[nodiscard]] float getFarPlane() const noexcept {return farPlane_;};
        [[nodiscard]] float getNearPlane() const noexcept {return nearPlane_;};


        void setLeft(const float left) noexcept {left_ = left;};
        void setRight(const float right) noexcept {right_ = right;};
        void setTop(const float top) noexcept {top_ = top;};
        void setBottom(const float bottom) noexcept {bottom_ = bottom;};
        void setFarPlane(const float farPlane) noexcept {farPlane_ = farPlane;};
        void setNearPlane(const float nearPlane) noexcept {nearPlane_ = nearPlane;};

        [[nodiscard]] glm::mat4 getProjectionMatrix() const noexcept override {return projectionMatrix_;};

        void render(const glm::mat4& modelViewMatrix) override;
        void onResize(int width, int height) noexcept override;


    protected:
        float left_;
        float right_;
        float top_;
        float bottom_;
        float nearPlane_;
        float farPlane_;

        glm::mat4 projectionMatrix_= {1.0};
    };
}
