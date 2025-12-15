/**
 * @file ovoreader.cpp
 * @brief Implementazione del parser per file OVO.
 */

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
#include "light_number_exception.h"
#include "omnidirectionallight.h"
#include "spotlight.h"
#include "texture.h"

using namespace std;
using namespace Eng;

// Strutture interne per il parsing (replicate da formato OVO)
/**
 * @brief Carica una scena da un file OVO.
 * @details Apre il file binario, itera sui chunk e ricostruisce il grafo della scena utilizzando uno stack per gestire la gerarchia padre-figlio.
 * @param path Percorso del file da caricare.
 * @return Puntatore al nodo radice della scena.
 */
std::shared_ptr<Node> OvoReader::load(const std::string& path) {

    // Mappa per materiali
    /** @brief cache dei materiali caricati per evitare duplicazioni e permettere il riutilizzo per nome */
    std::unordered_map<std::string, std::shared_ptr<Material>> materials;

    // Stack per gestire la gerarchia durante il parsing
    /** @details stack fondamentale per la ricostruzione dell'albero: tiene traccia dei nodi aperti in attesa di figli */
    std::stack<NodeInfo> hierarchyStack;

    // Nodo radice
    auto root = std::make_shared<Node>();
    root->setName("SceneRoot");
    /** @details inizializziamo lo stack con la radice che accetta infiniti figli (UINT_MAX) per fungere da contenitore base */
    hierarchyStack.push(NodeInfo(root, UINT_MAX)); // Root può avere infiniti figli

    FILE* dat = fopen(path.c_str(), "rb");
    if (!dat) {
        std::cerr << "ERROR: unable to open file '" << path << "'" << std::endl;
        return nullptr;
    }

    unsigned int chunkId, chunkSize;
    /** @brief ciclo principale di parsing dei chunk finché non finisce il file */
    while (true) {
        /** @details lettura dell'header del chunk: ID identificativo e dimensione in byte */
        fread(&chunkId, sizeof(unsigned int), 1, dat);
        if (feof(dat)) break;
        fread(&chunkSize, sizeof(unsigned int), 1, dat);

        // Leggi intero chunk in memoria
        /** @details caricamento dell'intero payload del chunk in un buffer temporaneo per l'elaborazione sicura in memoria */
        std::vector<char> buffer(chunkSize);
        if (fread(buffer.data(), 1, chunkSize, dat) != chunkSize) {
            fclose(dat);
            return nullptr;
        }
        char* data = buffer.data();
        unsigned int position = 0;

        /** @brief switch sul tipo di chunk per il parsing specifico dei dati */
        switch (static_cast<OvObject::Type>(chunkId)) {

            // ==========================
            // OBJECT (Version info)
            // ==========================
        case OvObject::Type::OBJECT: {
            unsigned int versionId;
            memcpy(&versionId, data + position, sizeof(unsigned int));
            position += sizeof(unsigned int);
            break;
        }


        case OvObject::Type::MATERIAL: {
            /** @brief lettura dati identificativi materiale */
            char name[FILENAME_MAX];
            strcpy(name, data + position);
            position += (unsigned int)strlen(name) + 1;

            /** @brief lettura proprietà cromatiche del materiale */
            glm::vec3 emission, albedo;
            memcpy(&emission, data + position, sizeof(glm::vec3)); position += sizeof(glm::vec3);
            memcpy(&albedo, data + position, sizeof(glm::vec3)); position += sizeof(glm::vec3);

            /** @brief lettura proprietà fisiche PBR */
            float roughness, metalness, alpha;
            memcpy(&roughness, data + position, sizeof(float)); position += sizeof(float);
            memcpy(&metalness, data + position, sizeof(float)); position += sizeof(float);
            memcpy(&alpha, data + position, sizeof(float)); position += sizeof(float);

            char textureName[FILENAME_MAX];

            /** @details lettura nome texture diffuse/albedo */
            strcpy(textureName, data + position);
            position += static_cast<unsigned int>(strlen(textureName)) + 1;

            char normalMapName[FILENAME_MAX];
            strcpy(normalMapName, data + position);
            position += static_cast<unsigned int>(strlen(normalMapName)) + 1;

            // Salta height, roughness, metalness maps
            /** @details skip delle mappe non supportate in questo loader (height, roughness, metalness) avanzando il puntatore */
            position += static_cast<unsigned int>(strlen(data + position)) + 1;
            position += static_cast<unsigned int>(strlen(data + position)) + 1;
            position += static_cast<unsigned int>(strlen(data + position)) + 1;

            /** @details creazione istanza materiale e conversione roughness in shininess per compatibilità */
            auto mat = std::make_shared<Material>(
                glm::vec4(emission, 1.0f),
                glm::vec4(albedo, 1.0f),
                glm::vec4(albedo, 1.0f),
                glm::vec4(1.0f),
                (1.0f - roughness) * 128.0f,
                textureName
            );

            /** @brief salvataggio materiale nella mappa per riutilizzo futuro nelle mesh */
            materials[name] = mat;
            break;
        }


        case OvObject::Type::NODE: {
            /** @brief lettura nome del nodo */
            char name[FILENAME_MAX];
            strcpy(name, data + position);
            position += static_cast<unsigned int>(strlen(name)) + 1;

            /** @brief lettura matrice di trasformazione locale */
            glm::mat4 matrix;
            memcpy(&matrix, data + position, sizeof(glm::mat4));
            position += sizeof(glm::mat4);

            /** @brief lettura numero figli attesi per questo nodo */
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
            /** @details gestione gerarchia: se c'è un genitore attivo nello stack, aggiungiamo il nodo corrente come figlio */
            if (!hierarchyStack.empty()) {
                NodeInfo& parent = hierarchyStack.top();
                parent.node->addChildren(node);
                parent.childrenAdded++;

                // Pulisci lo stack se il genitore ha finito i figli
                /** @details verifica completamento padre: se il nodo in cima ha ricevuto tutti i figli attesi, lo rimuoviamo dallo stack (pop) */
                while (!hierarchyStack.empty() &&
                    hierarchyStack.top().childrenAdded >= hierarchyStack.top().childrenCount) {
                    hierarchyStack.pop();
                }
            }

            // Se questo nodo ha figli, mettilo nello stack
            /** @brief push nello stack: se il nodo corrente aspetta figli, diventa il nuovo genitore attivo */
            if (children > 0) {
                hierarchyStack.push(NodeInfo(node, children));
            }

            break;
        }


        case OvObject::Type::MESH:
        case OvObject::Type::SKINNED: {
            /** @brief parsing dati mesh, inclusi nome e matrice */
            char name[FILENAME_MAX];
            strcpy(name, data + position);
            position += static_cast<unsigned int>(strlen(name)) + 1;

            glm::mat4 matrix;
            memcpy(&matrix, data + position, sizeof(glm::mat4));
            position += sizeof(glm::mat4);

            unsigned int children;
            memcpy(&children, data + position, sizeof(unsigned int));
            position += sizeof(unsigned int);

            char targetName[FILENAME_MAX];
            strcpy(targetName, data + position);
            position += static_cast<unsigned int>(strlen(targetName)) + 1;

            unsigned char subtype;
            memcpy(&subtype, data + position, sizeof(unsigned char));
            position += sizeof(unsigned char);

            char matName[FILENAME_MAX];
            strcpy(matName, data + position);
            position += static_cast<unsigned int>(strlen(matName)) + 1;

            // Salta radius e bbox
            /** @details skip dei dati di culling (raggio e bounding box) non utilizzati */
            position += sizeof(float) + sizeof(glm::vec3) * 2;

            unsigned char hasPhysics;
            memcpy(&hasPhysics, data + position, sizeof(unsigned char));
            position += sizeof(unsigned char);
            /** @details se presente, saltiamo l'intero blocco dati della fisica calcolando l'offset in byte */
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
            /** @brief caricamento del solo primo livello di dettaglio (LOD 0), ignorando i successivi */
            for (unsigned int l = 0; l < 1; l++) {
                unsigned int nVertices, nFaces;
                memcpy(&nVertices, data + position, sizeof(unsigned int)); position += sizeof(unsigned int);
                memcpy(&nFaces, data + position, sizeof(unsigned int));    position += sizeof(unsigned int);

                tempVertices.reserve(nVertices);
                tempNormals.reserve(nVertices);
                tempUvs.reserve(nVertices);

                /** @brief iterazione sui vertici e decompressione dati */
                for (unsigned int c = 0; c < nVertices; c++) {
                    glm::vec3 v;
                    memcpy(&v, data + position, sizeof(glm::vec3));
                    position += sizeof(glm::vec3);
                    tempVertices.push_back(v);

                    /** @details decompressione normali compattate (Snorm3x10_1x2) */
                    unsigned int normData;
                    memcpy(&normData, data + position, sizeof(unsigned int)); position += sizeof(unsigned int);
                    glm::vec4 norm = glm::unpackSnorm3x10_1x2(normData);
                    tempNormals.push_back(glm::vec3(norm));

                    /** @details decompressione coordinate UV (Half2x16) */
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

                /** @brief ricostruzione geometria de-indicizzata basata sulle facce (triangoli) */
                for (unsigned int c = 0; c < nFaces; c++) {
                    unsigned int face[3];
                    memcpy(face, data + position, sizeof(unsigned int) * 3);
                    position += sizeof(unsigned int) * 3;

                    for (unsigned int i : face) {
                        finalVertices.push_back(tempVertices[i]);
                        finalNormals.push_back(tempNormals[i]);
                        finalUvs.push_back(tempUvs[i]);
                    }
                }

                /** @brief creazione oggetto Mesh e associazione materiale tramite lookup nella mappa */
                auto mesh = std::make_shared<Mesh>(finalVertices, matName, materials[matName]);
                mesh->setName(name);
                mesh->setMatrix(matrix);
                mesh->setNormals(finalNormals);
                mesh->setUv_coords(finalUvs);

                // Aggiungi al genitore corrente
                /** @details collegamento mesh al nodo genitore attivo nello stack */
                if (!hierarchyStack.empty()) {
                    NodeInfo& parent = hierarchyStack.top();
                    parent.node->addChildren(mesh);
                    parent.childrenAdded++;

                    // Pulisci lo stack
                    /** @details controllo ricorsivo di chiusura nodi completati */
                    while (!hierarchyStack.empty() &&
                        hierarchyStack.top().childrenAdded >= hierarchyStack.top().childrenCount) {
                        hierarchyStack.pop();
                    }
                }

                // Se questa mesh ha figli, mettila nello stack
                /** @details anche le mesh possono avere figli (es. sub-mesh), quindi gestiamo lo stack anche qui */
                if (children > 0) {
                    hierarchyStack.push(NodeInfo(mesh, children));
                }
            }

            break;
        }



         case OvObject::Type::LIGHT: {
             /** @brief lettura nome e matrice della luce */
             char name[FILENAME_MAX];
             strcpy(name, data + position);
             position += static_cast<unsigned int>(strlen(name)) + 1;

             glm::mat4 matrix;
             memcpy(&matrix, data + position, sizeof(glm::mat4));
             position += sizeof(glm::mat4);

             unsigned int children;
             memcpy(&children, data + position, sizeof(unsigned int));
             position += sizeof(unsigned int);

             char targetName[FILENAME_MAX];
             strcpy(targetName, data + position);
             position += static_cast<unsigned int>(strlen(targetName)) + 1;

             unsigned char subtype;
             memcpy(&subtype, data + position, sizeof(unsigned char));
             position += sizeof(unsigned char);

             /** @brief lettura parametri specifici della luce: colore, raggio, direzione, cutoff */
             glm::vec3 color;
             memcpy(&color, data + position, sizeof(glm::vec3));
             position += sizeof(glm::vec3);
             float radius;
             memcpy(&radius, data + position, sizeof(float));
             position += sizeof(float);
             glm::vec3 dir;
             memcpy(&dir, data + position, sizeof(glm::vec3));
             position += sizeof(glm::vec3);
             float cutoff;
             memcpy(&cutoff, data + position, sizeof(float));
             position += sizeof(float);
             float exponent;
             memcpy(&exponent, data + position, sizeof(float));
             position += sizeof(float);

             position += sizeof(unsigned char) * 2;

             std::shared_ptr<Light> light;

             /** @brief factory method per istanziare il tipo corretto di luce (Directional, Omni, Spot) */
             switch (static_cast<OvLight::Subtype>(subtype)) {
             case OvLight::Subtype::DIRECTIONAL: {
                 try {
                     auto l = DirectionalLight::createDirectionalLight();
                     l->setDirection(glm::normalize(dir));
                     light = l;
                 } catch (const LightNumberExceededException& e) {
                     std::cerr << "LightNumberExceededException: " << e.what() << std::endl;
                 }
                 break;
             }
             case OvLight::Subtype::OMNI: {
                 try {
                     auto l = OmnidirectionalLight::createOmnidirectionalLight();
                     l->setRadius(radius);
                     light = l;
                 } catch (const LightNumberExceededException& e) {
                     std::cerr << "LightNumberExceededException: " << e.what() << std::endl;
                 }
                 break;
             }
             case OvLight::Subtype::SPOT: {
                 try {
                     auto l = Spotlight::createSpotLight();
                     l->setDirection(glm::normalize(dir));
                     l->setCutoff(cutoff);
                     l->setExponent(exponent);
                     l->setRadius(radius);
                     light = l;
                 } catch (const LightNumberExceededException& e) {
                     std::cerr << "LightNumberExceededException: " << e.what() << std::endl;
                 }
                 break;
             }
             default:
                 try {
                     light = OmnidirectionalLight::createOmnidirectionalLight();

                 } catch (const LightNumberExceededException& e) {
                     std::cerr << "LightNumberExceededException: " << e.what() << std::endl;
                 }

                 break;
             }

             light->setName(name);
             light->setMatrix(matrix);
             light->setDiffuse(color);
             light->setSpecular(color);
             light->setAmbient(color * 0.1f);

             /** @details collegamento luce al grafo della scena e aggiornamento stack genitori */
             if (!hierarchyStack.empty()) {
                 NodeInfo& parent = hierarchyStack.top();
                 parent.node->addChildren(light);
                 parent.childrenAdded++;

                 while (!hierarchyStack.empty() &&
                     hierarchyStack.top().childrenAdded >= hierarchyStack.top().childrenCount) {
                     hierarchyStack.pop();
                 }
             }

                /// @details se ha dei figli va messo in cima allo stack
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