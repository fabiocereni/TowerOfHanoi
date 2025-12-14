#include "setup.h"
#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Costruttore della classe Setup
 * @details Inizializza i percorsi e la lista di esclusione
 */
Setup::Setup(Eng::Base& eng, const std::string& projectDirName) : eng_(eng) {
    const std::filesystem::path sourceFile = __FILE__;
    projectExportDir_ = sourceFile.parent_path().parent_path() / projectDirName;

    excludedList_ = {
        "muro1", "muro2", "muro3", "muro4", "soffitto", "pavimento" ,"lampadario"
    };
}

/**
 * @brief Carica la scena 3D
 * @param ovoFileName Nome del file .ovo
 * @return std::shared_ptr<Eng::Node> Nodo radice della scena
 */
std::shared_ptr<Eng::Node> Setup::loadScene(const std::string& ovoFileName) {
    const std::filesystem::path modelPath = projectExportDir_ / ovoFileName;
    sceneRoot_ = eng_.load(modelPath.string());
    return sceneRoot_;
}


/**
 * @brief Inizializza la luce del lampadario
 */
std::shared_ptr<Eng::Light> Setup::initLampadario() const {
    const std::shared_ptr<Eng::Mesh> lampadarioNode = std::dynamic_pointer_cast<Eng::Mesh>(Eng::Base::findByName(sceneRoot_, "lampadario"));

    if (!lampadarioNode) {
        std::cerr << ">> ATTENZIONE: Nodo 'lampadario' non trovato." << std::endl;
        return nullptr;
    }

    const auto lampadarioLight = eng_.createOmnidirectionalLight();

    lampadarioLight->setName("lampadario_light");
    lampadarioLight->setActive(true);

    // @brief Appendi la luce al nodo lampadario
    lampadarioNode->addChildren(lampadarioLight);

    return lampadarioLight;
}

/**
 * @brief Crea la camera frontale
 */
std::shared_ptr<Eng::Camera> Setup::createFrontTableCamera() const {
    const auto frontTableCam = eng_.createPerspectiveCamera(40, 800.f / 600.f, 10.0f, 20000.0f);
    constexpr auto frontTablePosition = glm::vec3(0.0f, 2000.0f, 700.0f);

    const glm::mat4 frontTableCamWorldMatrix = glm::translate(glm::mat4(1.0f), frontTablePosition) *
        glm::rotate(glm::mat4(1.0f), glm::radians(-40.0f), glm::vec3(1.0f, 0, 0));

    frontTableCam->setMatrix(frontTableCamWorldMatrix);
    return frontTableCam;
}

/**
 * @brief Crea la camera superiore
 */
std::shared_ptr<Eng::Camera> Setup::createTopTableCamera() const {
    const auto topTableCam = eng_.createPerspectiveCamera(40, 800.f / 600.f, 10.0f, 20000.0f);
    constexpr auto topTableCamPosition = glm::vec3(0.0f, 3000.0f, -700.0f);

    const glm::mat4 cam2WorldMatrix =
        glm::translate(glm::mat4(1.0f), topTableCamPosition) *
        glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));

    topTableCam->setMatrix(cam2WorldMatrix);
    return topTableCam;
}

/**
 * @brief Crea la camera dinamica
 */
std::shared_ptr<Eng::Camera> Setup::createDynamicCamera() const {
    return eng_.createPerspectiveCamera(80, 1800.f / 1000.f, 10.0f, 20000.0f);
}

/**
 * @brief Helper per calcolare vertici in world space
 */
void Setup::computeWorldOrientedVertexes(const std::vector<glm::vec3>& vertexes,
                                  const std::shared_ptr<Eng::Node>& node,
                                  std::vector<glm::vec3>& out) const {
    const glm::mat4 world = node->getWorldMatrix();
    for (const auto& v : vertexes) {
        out.push_back(glm::vec3(world * glm::vec4(v, 1.0f)));
    }
}

/**
 * @brief Calcola i limiti della stanza per la camera
 */
void Setup::computeDynamicCameraLimits() {
    std::vector<glm::vec3> room_vertexes;
    const std::vector<std::string> limits = {"pavimento", "soffitto"};

    for(const auto& name : limits) {
        const auto node = Eng::Base::findByName(sceneRoot_, name);
        if(node) {
             // @details Cast a mesh per ottenere i vertici
             if(const auto mesh = std::dynamic_pointer_cast<Eng::Mesh>(node)) {
                 computeWorldOrientedVertexes(mesh->getVertexes(), node, room_vertexes);
             }
        }
    }

    for (auto& v : room_vertexes) {
        minX_ = std::min(minX_, v.x);
        maxX_ = std::max(maxX_, v.x);
        minY_ = std::min(minY_, v.y);
        maxY_ = std::max(maxY_, v.y);
        minZ_ = std::min(minZ_, v.z);
        maxZ_ = std::max(maxZ_, v.z);
    }
}

/**
 * @brief Aggiorna la matrice della camera dinamica
 */
void Setup::updateDynamicCameraMatrix(const std::shared_ptr<Eng::Camera>& cam) {
    constexpr float limitation_bound = 500.0f;

    // @brief Clamp della posizione
    dynamicCamPos_.x = glm::clamp(dynamicCamPos_.x, minX_ + limitation_bound, maxX_ - limitation_bound);
    dynamicCamPos_.y = glm::clamp(dynamicCamPos_.y, minY_ + limitation_bound, maxY_ - limitation_bound);
    dynamicCamPos_.z = glm::clamp(dynamicCamPos_.z, minZ_ + limitation_bound, maxZ_ - limitation_bound);

    const glm::mat4 x_rot = glm::rotate(glm::mat4(1.0f), glm::radians(dynamicCamXAngle_), glm::vec3(0, 1, 0));
    const glm::mat4 y_rot = glm::rotate(glm::mat4(1.0f), glm::radians(dynamicCamYAngle_), glm::vec3(1, 0, 0));
    const glm::mat4 trans = glm::translate(glm::mat4(1.0f), dynamicCamPos_);

    cam->setMatrix(trans * x_rot * y_rot);
}


/**
 * @brief Crea la luce dinamica
 */
std::shared_ptr<Eng::Node> Setup::createDynamicLight() {
    dynamicLight_ = Eng::OmnidirectionalLight::createOmnidirectionalLight();
    dynamicLight_->setName("dynamic_light");
    dynamicLight_->setAmbient(dynamicLightColor_);
    dynamicLight_->setDiffuse(dynamicLightColor_);
    dynamicLight_->setSpecular(dynamicLightColor_);
    dynamicLight_->setRadius(1000.0f);
    dynamicLight_->setActive(false);

    const auto albero = std::dynamic_pointer_cast<Eng::Mesh>(Eng::Base::findByName(sceneRoot_, "albero"));


    lightPivotNode_ = std::make_shared<Eng::Node>();
    lightPivotNode_->setName("pivot");

    if (albero) {
        const auto posAlbero = glm::vec3(albero->getWorldMatrix()[3]);
        pivotBasePos_ = posAlbero;
        pivotY_ = posAlbero.y;
        pivotAngle_ = 0.0f;

        lightPivotNode_->setMatrix(glm::mat4(1.0f));
        sceneRoot_->addChildren(lightPivotNode_);

        dynamicLight_->setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(230.0f, 0.0f, 0.0f)));
        lightPivotNode_->addChildren(dynamicLight_);
        dynamicLight_->setParent(lightPivotNode_);
    }

    return lightPivotNode_;
}

/**
 * @brief Calcola i limiti dell'albero
 */
void Setup::computeTreeHeightLimits() {
    const auto albero = Eng::Base::findByName(sceneRoot_, "albero");
    const auto cone0 = std::dynamic_pointer_cast<Eng::Mesh>(Eng::Base::findByName(sceneRoot_, "Cone"));
    const auto cone1 = std::dynamic_pointer_cast<Eng::Mesh>(Eng::Base::findByName(sceneRoot_, "Cone_001"));
    const auto cone2 = std::dynamic_pointer_cast<Eng::Mesh>(Eng::Base::findByName(sceneRoot_, "Cone_002"));

    if (!albero || (!cone0 && !cone2)) {
        minTreeY_ = maxTreeY_ = pivotY_;
        return;
    }

    auto processMesh = [&](const std::shared_ptr<Eng::Mesh>& mesh) {
        const auto& verts = mesh->getVertexes();
        glm::mat4 globalTransform = albero->getMatrix() * mesh->getMatrix();
        for (const auto& v : verts) {
            glm::vec4 worldV = globalTransform * glm::vec4(v, 1.0f);
            if (worldV.y < minTreeY_) minTreeY_ = worldV.y;
            if (worldV.y > maxTreeY_) maxTreeY_ = worldV.y;
        }
    };

    if (cone0) processMesh(cone0);
    if (cone2) processMesh(cone2);

    if (minTreeY_ == std::numeric_limits<float>::max()) {
         minTreeY_ = maxTreeY_ = pivotY_;
    }
}

/**
 * @brief Aggiorna animazione luce
 */
void Setup::updateDynamicLightAnimation() {
    if (!lightPivotNode_) return;

    constexpr float intensity = 5.0f;
    constexpr float speed = 4.0f;

    if (lightGoingUp_) {
        pivotY_ += speed;
        dynamicLightColor_ = glm::vec3(0.0f, intensity, 0.0f);
    } else {
        pivotY_ -= speed;
        dynamicLightColor_ = glm::vec3(intensity, 0.0f, 0.0f);
    }

    dynamicLight_->setAmbient(dynamicLightColor_);
    dynamicLight_->setDiffuse(dynamicLightColor_);
    dynamicLight_->setSpecular(dynamicLightColor_);

    if (pivotY_ >= maxTreeY_) {
        pivotY_ = maxTreeY_;
        lightGoingUp_ = false;
    } else if (pivotY_ <= minTreeY_) {
        pivotY_ = minTreeY_;
        lightGoingUp_ = true;
    }

    pivotAngle_ += 5.0f;

    glm::mat4 m(1.0f);
    m = glm::translate(m, glm::vec3(pivotBasePos_.x, pivotY_, pivotBasePos_.z));
    m = m * glm::rotate(glm::mat4(1.0f), glm::radians(pivotAngle_), glm::vec3(0, 1, 0));
    lightPivotNode_->setMatrix(m);
}

void Setup::updateLampadario() const noexcept {
    const auto lamp = std::dynamic_pointer_cast<Eng::Mesh>(Eng::Base::findByName(sceneRoot_ ,"lampadario"));
    const auto lamp_light = std::dynamic_pointer_cast<Eng::Light>(Eng::Base::findByName(sceneRoot_ ,"lampadario_light"));

    if (lamp && lamp_light) {

        if (lamp_light->isActive()) {
            lamp_light->setAmbient(glm::vec3(4.5f));
            lamp_light->setDiffuse(glm::vec3(4.5f));
            lamp_light->setSpecular(glm::vec3(4.5f));
            lamp_light->setRadius(3000.0f);
            lamp->getMaterial()->setEmission(glm::vec4(1.0f));
        } else {
            lamp->getMaterial()->setEmission(glm::vec4(0.0f));
        }
    }
}

std::shared_ptr<Eng::Camera> Setup::createAndReturnOrthoCamera(const int height, const int width) {
    return std::make_shared<Eng::Orthographic_Camera>(0.0f, static_cast<float>(width),
                                                                static_cast<float>(height),
                                                                0.0f, -1.0f, 1.0f);
}



void Setup::moveDynamicCam(const float dynamic_cam_speed) {
    // @details Calcola la direzione forward basata sugli angoli correnti
    const glm::mat4 orientation = glm::rotate(glm::mat4(1.0f), glm::radians(dynamicCamXAngle_), glm::vec3(0, 1, 0)) *
                                  glm::rotate(glm::mat4(1.0f), glm::radians(dynamicCamYAngle_), glm::vec3(1, 0, 0));
    const glm::vec3 forwardDir = -glm::vec3(orientation[2]);
    dynamicCamPos_ += forwardDir * dynamic_cam_speed;
}

void Setup::rotateDynamicCamX(const float dynamic_cam_speed) {
    dynamicCamXAngle_ += dynamic_cam_speed;
}

void Setup::rotateDynamicCamY(const float dynamic_cam_speed) {
    dynamicCamYAngle_ += dynamic_cam_speed;
}

void Setup::liftDynamicCam(const float dynamic_cam_speed) {
    dynamicCamPos_.y += dynamic_cam_speed;
}