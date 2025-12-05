#include "ovoreader.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/packing.hpp>
#include <glm/gtc/type_ptr.hpp>

// C++
#include <vector>
#include <iostream>
#include <cstring>
#include <unordered_map>
#include <stack>

// ENGINE
#include "mesh.h"
#include "material.h"
#include "light.h"
#include "directionallight.h"
#include "omnidirectionallight.h"
#include "spotlight.h"
#include "texture.h"

using namespace std;
using namespace Eng;

// Strutture interne per il parsing (replicate da formato OVO)
namespace {
    class OvObject {
    public:
        enum class Type : int {
            OBJECT = 0, NODE, OBJECT2D, OBJECT3D, LIST, BUFFER, SHADER, TEXTURE, FILTER, MATERIAL,
            FBO, QUAD, BOX, SKYBOX, FONT, CAMERA, LIGHT, BONE, MESH, SKINNED, INSTANCED, PIPELINE,
            EMITTER, ANIM, PHYSICS, LAST,
        };
    };

    class OvMesh {
    public:
        enum class Subtype : int { DEFAULT = 0, NORMALMAPPED, TESSELLATED, LAST };
    };

    class OvLight {
    public:
        enum class Subtype : int { OMNI = 0, DIRECTIONAL, SPOT, LAST };
    };

    // Struttura per tenere traccia della gerarchia durante il parsing
    struct NodeInfo {
        std::shared_ptr<Node> node;
        unsigned int childrenCount;
        unsigned int childrenAdded;

        NodeInfo(std::shared_ptr<Node> n, unsigned int count)
            : node(n), childrenCount(count), childrenAdded(0) {
        }
    };
}

std::shared_ptr<Node> OvoReader::load(const std::string& path) {

    // Mappa per materiali
    std::unordered_map<std::string, std::shared_ptr<Material>> materials;

    // Stack per gestire la gerarchia durante il parsing
    std::stack<NodeInfo> hierarchyStack;

    // Nodo radice
    auto root = std::make_shared<Node>();
    root->setName("SceneRoot");
    hierarchyStack.push(NodeInfo(root, UINT_MAX)); // Root può avere infiniti figli

    FILE* dat = fopen(path.c_str(), "rb");
    if (!dat) {
        std::cerr << "ERROR: unable to open file '" << path << "'" << std::endl;
        return nullptr;
    }

    unsigned int chunkId, chunkSize;
    while (true) {
        fread(&chunkId, sizeof(unsigned int), 1, dat);
        if (feof(dat)) break;
        fread(&chunkSize, sizeof(unsigned int), 1, dat);

        // Leggi intero chunk in memoria
        std::vector<char> buffer(chunkSize);
        if (fread(buffer.data(), 1, chunkSize, dat) != chunkSize) {
            fclose(dat);
            return nullptr;
        }
        char* data = buffer.data();
        unsigned int position = 0;

        switch ((OvObject::Type)chunkId) {

            // ==========================
            // OBJECT (Version info)
            // ==========================
        case OvObject::Type::OBJECT: {
            unsigned int versionId;
            memcpy(&versionId, data + position, sizeof(unsigned int));
            position += sizeof(unsigned int);
            break;
        }

                                   // ==========================
                                   // MATERIAL
                                   // ==========================
        case OvObject::Type::MATERIAL: {
            char name[FILENAME_MAX];
            strcpy(name, data + position);
            position += (unsigned int)strlen(name) + 1;

            glm::vec3 emission, albedo;
            memcpy(&emission, data + position, sizeof(glm::vec3)); position += sizeof(glm::vec3);
            memcpy(&albedo, data + position, sizeof(glm::vec3)); position += sizeof(glm::vec3);

            float roughness, metalness, alpha;
            memcpy(&roughness, data + position, sizeof(float)); position += sizeof(float);
            memcpy(&metalness, data + position, sizeof(float)); position += sizeof(float);
            memcpy(&alpha, data + position, sizeof(float)); position += sizeof(float);

            char textureName[FILENAME_MAX];
                std::cout << textureName << std::endl;
            strcpy(textureName, data + position);
            position += (unsigned int)strlen(textureName) + 1;

            char normalMapName[FILENAME_MAX];
            strcpy(normalMapName, data + position);
            position += (unsigned int)strlen(normalMapName) + 1;

            // Salta height, roughness, metalness maps
            position += (unsigned int)strlen(data + position) + 1;
            position += (unsigned int)strlen(data + position) + 1;
            position += (unsigned int)strlen(data + position) + 1;

            auto mat = std::make_shared<Material>(
                glm::vec4(emission, 1.0f),
                glm::vec4(albedo, 1.0f),
                glm::vec4(albedo, 1.0f),
                glm::vec4(1.0f),
                (1.0f - roughness) * 128.0f,
                textureName
            );

            materials[name] = mat;
            break;
        }

                                     // ==========================
                                     // NODE
                                     // ==========================
        case OvObject::Type::NODE: {
            char name[FILENAME_MAX];
            strcpy(name, data + position);
            position += (unsigned int)strlen(name) + 1;

            glm::mat4 matrix;
            memcpy(&matrix, data + position, sizeof(glm::mat4));
            position += sizeof(glm::mat4);

            unsigned int children;
            memcpy(&children, data + position, sizeof(unsigned int));
            position += sizeof(unsigned int);

            char targetName[FILENAME_MAX];
            strcpy(targetName, data + position);
            position += (unsigned int)strlen(targetName) + 1;

            auto node = std::make_shared<Node>();
            node->setName(name);
            node->setMatrix(matrix);

            // Aggiungi al genitore corrente
            if (!hierarchyStack.empty()) {
                NodeInfo& parent = hierarchyStack.top();
                parent.node->addChildren(node);
                parent.childrenAdded++;

                // Pulisci lo stack se il genitore ha finito i figli
                while (!hierarchyStack.empty() &&
                    hierarchyStack.top().childrenAdded >= hierarchyStack.top().childrenCount) {
                    hierarchyStack.pop();
                }
            }

            // Se questo nodo ha figli, mettilo nello stack
            if (children > 0) {
                hierarchyStack.push(NodeInfo(node, children));
            }

            break;
        }

                                 // ==========================
                                 // MESH
                                 // ==========================
        case OvObject::Type::MESH:
        case OvObject::Type::SKINNED: {
            char name[FILENAME_MAX];
            strcpy(name, data + position);
            position += (unsigned int)strlen(name) + 1;

            glm::mat4 matrix;
            memcpy(&matrix, data + position, sizeof(glm::mat4));
            position += sizeof(glm::mat4);

            unsigned int children;
            memcpy(&children, data + position, sizeof(unsigned int));
            position += sizeof(unsigned int);

            char targetName[FILENAME_MAX];
            strcpy(targetName, data + position);
            position += (unsigned int)strlen(targetName) + 1;

            unsigned char subtype;
            memcpy(&subtype, data + position, sizeof(unsigned char));
            position += sizeof(unsigned char);

            char matName[FILENAME_MAX];
            strcpy(matName, data + position);
            position += (unsigned int)strlen(matName) + 1;

            // Salta radius e bbox
            position += sizeof(float) + sizeof(glm::vec3) * 2;

            unsigned char hasPhysics;
            memcpy(&hasPhysics, data + position, sizeof(unsigned char));
            position += sizeof(unsigned char);
            if (hasPhysics) {
                position += sizeof(unsigned char) * 4 + sizeof(glm::vec3) + sizeof(float) * 7 + sizeof(unsigned int) * 2 + sizeof(void*) * 2;
                position += sizeof(unsigned int);
            }

            unsigned int LODs;
            memcpy(&LODs, data + position, sizeof(unsigned int));
            position += sizeof(unsigned int);

            std::vector<glm::vec3> tempVertices;
            std::vector<glm::vec3> tempNormals;
            std::vector<glm::vec2> tempUvs;

            // Leggiamo solo il primo LOD
            for (unsigned int l = 0; l < 1; l++) {
                unsigned int nVertices, nFaces;
                memcpy(&nVertices, data + position, sizeof(unsigned int)); position += sizeof(unsigned int);
                memcpy(&nFaces, data + position, sizeof(unsigned int));    position += sizeof(unsigned int);

                tempVertices.reserve(nVertices);
                tempNormals.reserve(nVertices);
                tempUvs.reserve(nVertices);

                for (unsigned int c = 0; c < nVertices; c++) {
                    glm::vec3 v;
                    memcpy(&v, data + position, sizeof(glm::vec3));
                    position += sizeof(glm::vec3);
                    tempVertices.push_back(v);

                    unsigned int normData;
                    memcpy(&normData, data + position, sizeof(unsigned int)); position += sizeof(unsigned int);
                    glm::vec4 norm = glm::unpackSnorm3x10_1x2(normData);
                    tempNormals.push_back(glm::vec3(norm));

                    unsigned int texData;
                    memcpy(&texData, data + position, sizeof(unsigned int)); position += sizeof(unsigned int);
                    glm::vec2 uv = glm::unpackHalf2x16(texData);
                    tempUvs.push_back(uv);

                    // Skip tangent
                    position += sizeof(unsigned int);
                }

                std::vector<glm::vec3> finalVertices;
                std::vector<glm::vec3> finalNormals;
                std::vector<glm::vec2> finalUvs;

                for (unsigned int c = 0; c < nFaces; c++) {
                    unsigned int face[3];
                    memcpy(face, data + position, sizeof(unsigned int) * 3);
                    position += sizeof(unsigned int) * 3;

                    for (int i = 0; i < 3; i++) {
                        finalVertices.push_back(tempVertices[face[i]]);
                        finalNormals.push_back(tempNormals[face[i]]);
                        finalUvs.push_back(tempUvs[face[i]]);
                    }
                }

                auto mesh = std::make_shared<Mesh>(finalVertices, matName, materials[matName]);
                mesh->setName(name);
                mesh->setMatrix(matrix);
                mesh->setNormals(finalNormals);
                mesh->setUv_coords(finalUvs);

                // Aggiungi al genitore corrente
                if (!hierarchyStack.empty()) {
                    NodeInfo& parent = hierarchyStack.top();
                    parent.node->addChildren(mesh);
                    parent.childrenAdded++;

                    // Pulisci lo stack
                    while (!hierarchyStack.empty() &&
                        hierarchyStack.top().childrenAdded >= hierarchyStack.top().childrenCount) {
                        hierarchyStack.pop();
                    }
                }

                // Se questa mesh ha figli, mettila nello stack
                if (children > 0) {
                    hierarchyStack.push(NodeInfo(mesh, children));
                }
            }

            break;
        }

                                    // ==========================
                                    // LIGHT
                                    // ==========================
        case OvObject::Type::LIGHT: {
            char name[FILENAME_MAX];
            strcpy(name, data + position);
            position += (unsigned int)strlen(name) + 1;

            glm::mat4 matrix;
            memcpy(&matrix, data + position, sizeof(glm::mat4));
            position += sizeof(glm::mat4);

            unsigned int children;
            memcpy(&children, data + position, sizeof(unsigned int));
            position += sizeof(unsigned int);

            char targetName[FILENAME_MAX];
            strcpy(targetName, data + position);
            position += (unsigned int)strlen(targetName) + 1;

            unsigned char subtype;
            memcpy(&subtype, data + position, sizeof(unsigned char));
            position += sizeof(unsigned char);

            glm::vec3 color; memcpy(&color, data + position, sizeof(glm::vec3)); position += sizeof(glm::vec3);
            float radius; memcpy(&radius, data + position, sizeof(float)); position += sizeof(float);
            glm::vec3 dir; memcpy(&dir, data + position, sizeof(glm::vec3)); position += sizeof(glm::vec3);
            float cutoff; memcpy(&cutoff, data + position, sizeof(float)); position += sizeof(float);
            float exponent; memcpy(&exponent, data + position, sizeof(float)); position += sizeof(float);

            position += sizeof(unsigned char) * 2;

            std::shared_ptr<Light> light;

            switch ((OvLight::Subtype)subtype) {
            case OvLight::Subtype::DIRECTIONAL: {
                auto l = std::make_shared<DirectionalLight>();
                l->setDirection(glm::normalize(dir));
                light = l;
                break;
            }
            case OvLight::Subtype::OMNI: {
                auto l = std::make_shared<OmnidirectionalLight>();
                l->setRadius(radius);
                light = l;
                break;
            }
            case OvLight::Subtype::SPOT: {
                auto l = std::make_shared<Spotlight>();
                l->setDirection(glm::normalize(dir));
                l->setCutoff(cutoff);
                l->setExponent(exponent);
                l->setRadius(radius);
                light = l;
                break;
            }
            default: light = std::make_shared<OmnidirectionalLight>(); break;
            }

            light->setName(name);
            light->setMatrix(matrix);
            light->setDiffuse(color);
            light->setSpecular(color);
            light->setAmbient(color * 0.1f);

            // Aggiungi al genitore corrente
            if (!hierarchyStack.empty()) {
                NodeInfo& parent = hierarchyStack.top();
                parent.node->addChildren(light);
                parent.childrenAdded++;

                // Pulisci lo stack
                while (!hierarchyStack.empty() &&
                    hierarchyStack.top().childrenAdded >= hierarchyStack.top().childrenCount) {
                    hierarchyStack.pop();
                }
            }

            // Se questa luce ha figli, mettila nello stack
            if (children > 0) {
                hierarchyStack.push(NodeInfo(light, children));
            }

            break;
        }

        default:
            break;
        }
    }
    fclose(dat);

    return root;
}