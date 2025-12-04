#pragma once

#include "engine_api.h"
#include "engine.h"
#include "light.h"
#include "material.h"
#include "mesh.h"
#include "node.h"

#include <vector>
#include <memory>

namespace Eng {

class ENG_API Scene {
public:
    Scene() noexcept = default;
    Scene(std::vector<std::shared_ptr<Mesh>> meshes,
          std::vector<std::shared_ptr<Material>> materials,
          std::vector<std::shared_ptr<Light>> lights,
          std::vector<std::shared_ptr<Node>> nodes) noexcept;

    Scene(const Scene& other) noexcept;
    Scene(Scene&& other) noexcept;

    Scene& operator=(const Scene& other) noexcept;
    Scene& operator=(Scene&& other) noexcept;

    ~Scene() noexcept = default;


    [[nodiscard]] const std::vector<std::shared_ptr<Mesh>>& getMeshes() const noexcept {
        return meshes_;
    }

    [[nodiscard]] const std::vector<std::shared_ptr<Material>>& getMaterials() const noexcept {
        return materials_;
    }

    [[nodiscard]] const std::vector<std::shared_ptr<Light>>& getLights() const noexcept {
        return lights_;
    }

    [[nodiscard]] const std::vector<std::shared_ptr<Node>>& getNodes() const noexcept {
        return nodes_;
    }


    [[nodiscard]] std::vector<std::shared_ptr<Mesh>>& getMeshes() noexcept {
        return meshes_;
    }

    [[nodiscard]] std::vector<std::shared_ptr<Material>>& getMaterials() noexcept {
        return materials_;
    }

    [[nodiscard]] std::vector<std::shared_ptr<Light>>& getLights() noexcept {
        return lights_;
    }

    [[nodiscard]] std::vector<std::shared_ptr<Node>>& getNodes() noexcept {
        return nodes_;
    }

    void setMeshes(std::vector<std::shared_ptr<Mesh>> meshes) noexcept {
        meshes_ = std::move(meshes);
    }

    void setMaterials(std::vector<std::shared_ptr<Material>> materials) noexcept {
        materials_ = std::move(materials);
    }

    void setLights(std::vector<std::shared_ptr<Light>> lights) noexcept {
        lights_ = std::move(lights);
    }

    void setNodes(std::vector<std::shared_ptr<Node>> nodes) noexcept {
        nodes_ = std::move(nodes);
    }

    void addMesh(const std::shared_ptr<Mesh>& mesh) noexcept {
        meshes_.push_back(mesh);
    }

    void addMaterial(const std::shared_ptr<Material>& material) noexcept {
        materials_.push_back(material);
    }

    void addLight(const std::shared_ptr<Light>& light) noexcept {
        lights_.push_back(light);
    }

    void addNode(const std::shared_ptr<Node>& node) noexcept {
        nodes_.push_back(node);
    }

private:
    std::vector<std::shared_ptr<Mesh>> meshes_;
    std::vector<std::shared_ptr<Material>> materials_;
    std::vector<std::shared_ptr<Light>> lights_;
    std::vector<std::shared_ptr<Node>> nodes_;
};

}
