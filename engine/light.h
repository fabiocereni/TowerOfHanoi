#pragma once
#include "engine_api.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "node.h"

namespace Eng {

class ENG_API Light : public Node {
public:
    explicit Light(int index) noexcept;
    virtual ~Light() noexcept = default;

    void render(const glm::mat4& modelViewMatrix) override = 0;


    void setAmbient (const glm::vec3& c) noexcept { ambient_  = c; }
    void setDiffuse (const glm::vec3& c) noexcept { diffuse_  = c; }
    void setSpecular(const glm::vec3& c) noexcept { specular_ = c; }
    void setRadius      (const float r) noexcept { radius_ = r; }
    void setConstantAtt (const float c) noexcept { attConst_ = c; }
    void setLinearAtt   (const float l) noexcept { attLinear_ = l; }
    void setQuadraticAtt(const float q) noexcept { attQuad_ = q; }


    [[nodiscard]] static int getLightNumber() noexcept { return lightNumber_; }


    [[nodiscard]] glm::vec3 getAmbient()  const noexcept { return ambient_; }
    [[nodiscard]] glm::vec3 getDiffuse()  const noexcept { return diffuse_; }
    [[nodiscard]] glm::vec3 getSpecular() const noexcept { return specular_; }
    [[nodiscard]] float getRadius()       const noexcept { return radius_; }
    [[nodiscard]] float getConstantAtt()  const noexcept { return attConst_; }
    [[nodiscard]] float getLinearAtt()    const noexcept { return attLinear_; }
    [[nodiscard]] float getQuadraticAtt() const noexcept { return attQuad_; }

protected:
    static int lightNumber_;
    int index_;

    glm::vec3 ambient_  {0.0f};
    glm::vec3 diffuse_  {1.0f};
    glm::vec3 specular_ {1.0f};


    float radius_      = 1.0f;  // for attenuation


    float attConst_    = 1.0f;
    float attLinear_   = 0.0f;
    float attQuad_     = 0.0f;
};

}
