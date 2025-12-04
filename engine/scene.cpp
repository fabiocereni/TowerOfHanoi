#include "scene.h"
using namespace Eng;

Scene::Scene(std::vector<std::shared_ptr<Mesh>> meshes,
                  std::vector<std::shared_ptr<Material>> materials,
                  std::vector<std::shared_ptr<Light>> lights,
                  std::vector<std::shared_ptr<Node>> nodes) noexcept
        : meshes_{std::move(meshes)}
    , materials_{std::move(materials)}
    , lights_{std::move(lights)}
    , nodes_{std::move(nodes)} {}


    Scene::Scene(const Scene& other) noexcept
        : meshes_{other.meshes_}
    , materials_{other.materials_}
    , lights_{other.lights_}
    , nodes_{other.nodes_} {}



    Scene::Scene(Scene&& other) noexcept
        : meshes_{std::move(other.meshes_)}
    , materials_{std::move(other.materials_)}
    , lights_{std::move(other.lights_)}
    , nodes_{std::move(other.nodes_)} {}



    Scene& Scene::operator=(const Scene& other) noexcept {
        if (this != &other) {
            meshes_    = other.meshes_;
            materials_ = other.materials_;
            lights_    = other.lights_;
            nodes_     = other.nodes_;
        }
        return *this;
    }


    Scene& Scene::operator=(Scene&& other) noexcept {
        if (this != &other) {
            meshes_    = std::move(other.meshes_);
            materials_ = std::move(other.materials_);
            lights_    = std::move(other.lights_);
            nodes_     = std::move(other.nodes_);
        }
        return *this;
    }

