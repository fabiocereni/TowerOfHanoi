#include "scene.h"

using namespace Eng;

    void Scene::addMesh(const std::string& name, const std::shared_ptr<Mesh>& mesh) noexcept {
        meshesByName_[name] = mesh;
        nodesByName_[name]  = mesh;
    }

    void Scene::addMaterial(const std::string& name, const std::shared_ptr<Material>& material) noexcept {
        materialsByName_[name] = material;
    }

    void Scene::addLight(const std::string& name, const std::shared_ptr<Light>& light) noexcept {
        lightsByName_[name] = light;
        nodesByName_[name]  = light;
    }

    void Scene::addNode(const std::string& name, const std::shared_ptr<Node>& node) noexcept {
        nodesByName_[name] = node;
    }




    std::shared_ptr<Mesh> Scene::findMeshByName(const std::string& name) const noexcept {
        if (const auto it = meshesByName_.find(name); it != meshesByName_.end())
            return it->second;
        return nullptr;
    }

    std::shared_ptr<Material> Scene::findMaterialByName(const std::string& name) const noexcept {
        if (const auto it = materialsByName_.find(name); it != materialsByName_.end())
            return it->second;
        return nullptr;
    }

    std::shared_ptr<Light> Scene::findLightByName(const std::string& name) const noexcept {
        if (const auto it = lightsByName_.find(name); it != lightsByName_.end())
            return it->second;
        return nullptr;
    }

    std::shared_ptr<Node> Scene::findNodeByName(const std::string& name) const noexcept {
        if (const auto it = nodesByName_.find(name); it != nodesByName_.end())
            return it->second;
        return nullptr;
    }
