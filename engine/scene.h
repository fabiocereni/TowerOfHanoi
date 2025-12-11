#pragma once

#include "engine_api.h"
#include "node.h"
#include "mesh.h"
#include "light.h"
#include "material.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace Eng {

    class ENG_API Scene {
    public:
        Scene() noexcept = default;
        ~Scene() noexcept = default;

        void addMesh(const std::string& name, const std::shared_ptr<Mesh>& mesh) noexcept;
        void addMaterial(const std::string& name, const std::shared_ptr<Material>& material) noexcept;
        void addLight(const std::string& name, const std::shared_ptr<Light>& light) noexcept;
        void addNode(const std::string& name, const std::shared_ptr<Node>& node) noexcept;

        [[nodiscard]] std::shared_ptr<Mesh>     findMeshByName(const std::string& name) const noexcept;
        [[nodiscard]] std::shared_ptr<Material> findMaterialByName(const std::string& name) const noexcept;
        [[nodiscard]] std::shared_ptr<Light>    findLightByName(const std::string& name) const noexcept;
        [[nodiscard]] std::shared_ptr<Node>     findNodeByName(const std::string& name) const noexcept;

        [[nodiscard]] const auto& getMeshMap()     const noexcept { return meshesByName_; }
        [[nodiscard]] const auto& getMaterialMap() const noexcept { return materialsByName_; }
        [[nodiscard]] const auto& getLightMap()    const noexcept { return lightsByName_; }
        [[nodiscard]] const auto& getNodeMap()     const noexcept { return nodesByName_; }

    private:
        std::unordered_map<std::string, std::shared_ptr<Mesh>>     meshesByName_;
        std::unordered_map<std::string, std::shared_ptr<Material>> materialsByName_;
        std::unordered_map<std::string, std::shared_ptr<Light>>    lightsByName_;
        std::unordered_map<std::string, std::shared_ptr<Node>>     nodesByName_;
    };

}
