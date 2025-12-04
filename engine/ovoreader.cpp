//////////////
// #INCLUDE //
//////////////

   // GLM:
   #include <glm/glm.hpp>
   #include <glm/gtc/packing.hpp>

   // C/C++:
#include "ovoreader.h"

   #include <vector>
   #include <iostream>
   #include <iomanip>
   #include <limits.h>
#include <memory>
#include <ranges>
#include <unordered_map>

#include "directionallight.h"
#include "material.h"
#include "mesh.h"
#include "node.h"
#include "omnidirectionallight.h"
#include "spotlight.h"
using namespace std;



/////////////
// #DEFINE //
/////////////

   // General:
   constexpr auto APP_NAME = "OVO Reader v0.8.2r";

   // Macro for printing an OvMatrix4 to console:

   // Stripped-down redefinition of OvObject (just for the chunk IDs):
   class OvObject
   {
      public:
      enum class Type : int  ///< Type of entities
      {
         // Foundation types:
         OBJECT = 0,
         NODE,
         OBJECT2D,
         OBJECT3D,
         LIST,

         // Derived classes:
         BUFFER,
         SHADER,
         TEXTURE,
         FILTER,
         MATERIAL,
         FBO,
         QUAD,
         BOX,
         SKYBOX,
         FONT,
         CAMERA,
         LIGHT,
         BONE,
         MESH,	   // Keep them...
         SKINNED, // ...consecutive
         INSTANCED,
         PIPELINE,
         EMITTER,

         // Animation type
         ANIM,

         // Physics related:
         PHYSICS,

         // Terminator:
         LAST,
      };
   };

   // Stripped-down redefinition of OvMesh (just for the subtypes):
   class OvMesh
   {
   public:
      enum class Subtype : int ///< Kind of mesh
      {
         // Foundation types:
         DEFAULT = 0,
         NORMALMAPPED,
         TESSELLATED,

         // Terminator:
         LAST,
      };
   };

   // Stripped-down redefinition of OvLight (just for the subtypes):
   class OvLight
   {
      public:
      enum class Subtype : int ///< Kind of light
      {
         // Foundation types:
         OMNI = 0,
         DIRECTIONAL,
         SPOT,

         // Terminator:
         LAST,
      };
   };



//////////
// MAIN //
//////////

/**
 * Application entry point. For details on the file format, build and browse the Doxygen documentation.
 * @param file_path Path del file da aprire
 * @return error code
 */

// prima prendeva come parametri const int argc, char** argv (era la firma del main)
std::shared_ptr<Eng::Node> Eng::OvoParser::returnCompleteSceneTree(const std::string& file_path)
{
    std::shared_ptr<Node> root = nullptr;

    unordered_map<std::string, std::shared_ptr<Node>> nodeMap;
    std::unordered_map<std::string, std::shared_ptr<Material>> materialMap;
    std::unordered_map<std::string, std::shared_ptr<Node>> lightMap;
    std::unordered_map<std::string, std::shared_ptr<Mesh>> meshMap;

    FILE *dat = fopen(file_path.c_str(), "rb");
    if (!dat) return nullptr;

    unsigned int chunkId, chunkSize;

    while (fread(&chunkId, sizeof(unsigned int), 1, dat))
    {
        fread(&chunkSize, sizeof(unsigned int), 1, dat);

        char *data = new char[chunkSize];
        fread(data, 1, chunkSize, dat);

        unsigned int pos = 0;

        switch ((OvObject::Type)chunkId)
        {
        // =========================
        //        NODE
        // =========================
        case OvObject::Type::NODE:
        {
            char name[FILENAME_MAX];
            strcpy(name, data + pos);
            pos += strlen(name) + 1;

            glm::mat4 matrix;
            memcpy(&matrix, data + pos, sizeof(glm::mat4));
            pos += sizeof(glm::mat4);

            unsigned int children;
            memcpy(&children, data + pos, sizeof(unsigned int));
            pos += sizeof(unsigned int);

            // targetName (ignored)
            pos += strlen(data + pos) + 1;

            auto node = std::make_shared<Eng::Node>();
            node->setMatrix(matrix);
            node->setName(name);

            nodeMap[name] = node;
            if (!root)
                root = node;
        }
        break;

        // =========================
        //       MATERIAL
        // =========================
        case OvObject::Type::MATERIAL:
        {
            char name[FILENAME_MAX];
            strcpy(name, data + pos);
            pos += strlen(name) + 1;

            glm::vec3 emission;
            memcpy(&emission, data + pos, sizeof(glm::vec3));
            pos += sizeof(glm::vec3);

            glm::vec3 albedo;
            memcpy(&albedo, data + pos, sizeof(glm::vec3));
            pos += sizeof(glm::vec3);

            float roughness;
            memcpy(&roughness, data + pos, sizeof(float));
            pos += sizeof(float);

            float metalness;
            memcpy(&metalness, data + pos, sizeof(float));
            pos += sizeof(float);

            float transparency;
            memcpy(&transparency, data + pos, sizeof(float));
            pos += sizeof(float);

            char tex[FILENAME_MAX];
            strcpy(tex, data + pos);
            pos += strlen(tex) + 1;

            // normal map (ignored)
            pos += strlen(data + pos) + 1;

            auto mat = std::make_shared<Eng::Material>(
                glm::vec4(emission, 1.0f),
                glm::vec4(albedo, 1.0f),
                glm::vec4(albedo, 1.0f),
                glm::vec4(1,1,1,1),
                32.0f,
                tex
            );

            materialMap[name] = mat;
        }
        break;

        // =========================
        //         MESH
        // =========================
        case OvObject::Type::MESH:
        {
            char name[FILENAME_MAX];
            strcpy(name, data + pos);
            pos += strlen(name) + 1;

            glm::mat4 matrix;
            memcpy(&matrix, data + pos, sizeof(glm::mat4));
            pos += sizeof(glm::mat4);

            unsigned int children;
            memcpy(&children, data + pos, sizeof(unsigned int));
            pos += sizeof(unsigned int);

            // target (ignored)
            pos += strlen(data + pos) + 1;

            unsigned char subtype;
            memcpy(&subtype, data + pos, sizeof(unsigned char));
            pos += sizeof(unsigned char);

            char matName[FILENAME_MAX];
            strcpy(matName, data + pos);
            pos += strlen(matName) + 1;

            // radius (+ bounding box)
            pos += sizeof(float) + sizeof(glm::vec3) * 2;

            // physics flag
            unsigned char hasPhys;
            memcpy(&hasPhys, data + pos, sizeof(unsigned char));
            pos += sizeof(unsigned char);

            if (hasPhys)
            {
                // skip entire physics struct
                pos += sizeof(unsigned char) * 4 +
                       sizeof(glm::vec3) +
                       sizeof(float) * 7 +
                       sizeof(unsigned int) * 2 +
                       sizeof(void*) * 2;
            }

            // LOD count
            unsigned int LODs;
            memcpy(&LODs, data + pos, sizeof(unsigned int));
            pos += sizeof(unsigned int);

            std::vector<glm::vec3> verts;

            // use ONLY the first LOD
            // (your Mesh class does not support multiple)
            for (unsigned int l = 0; l < 1; l++)
            {
                unsigned int vcount, fcount;
                memcpy(&vcount, data + pos, sizeof(unsigned int));
                pos += sizeof(unsigned int);

                memcpy(&fcount, data + pos, sizeof(unsigned int));
                pos += sizeof(unsigned int);

                verts.reserve(vcount);

                for (unsigned int i = 0; i < vcount; i++)
                {
                    glm::vec3 v;
                    memcpy(&v, data + pos, sizeof(glm::vec3));
                    pos += sizeof(glm::vec3);

                    // skip compressed normal, uv, tangent
                    pos += sizeof(unsigned int) * 3;

                    verts.push_back(v);
                }

                // skip faces (engine doesn't use)
                pos += fcount * sizeof(unsigned int) * 3;
            }

            auto mesh = std::make_shared<Eng::Mesh>(
                verts,
                matName,
                materialMap.count(matName) ? materialMap[matName] : nullptr
            );

            mesh->setMatrix(matrix);
            mesh->setName(name);

            meshMap[name] = mesh;
        }
        break;

        // =========================
        //         LIGHT
        // =========================
        case OvObject::Type::LIGHT:
        {
            char name[FILENAME_MAX];
            strcpy(name, data + pos);
            pos += strlen(name) + 1;

            glm::mat4 matrix;
            memcpy(&matrix, data + pos, sizeof(glm::mat4));
            pos += sizeof(glm::mat4);

            unsigned int children;
            memcpy(&children, data + pos, sizeof(unsigned int));
            pos += sizeof(unsigned int);

            // target
            pos += strlen(data + pos) + 1;

            unsigned char subtype;
            memcpy(&subtype, data + pos, sizeof(unsigned char));
            pos += sizeof(unsigned char);

            glm::vec3 color;
            memcpy(&color, data + pos, sizeof(glm::vec3));
            pos += sizeof(glm::vec3);

            float radius;
            memcpy(&radius, data + pos, sizeof(float));
            pos += sizeof(float);

            glm::vec3 direction;
            memcpy(&direction, data + pos, sizeof(glm::vec3));
            pos += sizeof(glm::vec3);

            float cutoff;
            memcpy(&cutoff, data + pos, sizeof(float));
            pos += sizeof(float);

            float exponent;
            memcpy(&exponent, data + pos, sizeof(float));
            pos += sizeof(float);

            unsigned char castShadows;
            pos += sizeof(unsigned char);

            unsigned char volumetric;
            pos += sizeof(unsigned char);

            std::shared_ptr<Node> light;

            switch ((OvLight::Subtype)subtype)
            {
                case OvLight::Subtype::DIRECTIONAL:
                    light = std::make_shared<DirectionalLight>(color, direction);
                    break;

                case OvLight::Subtype::OMNI:
                    // light = std::make_shared<OmnidirectionalLight>(color, radius);
                    break;

                case OvLight::Subtype::SPOT:
                    // light = std::make_shared<Spotlight>(color, direction, cutoff, exponent);
                    break;
            }

            light->setMatrix(matrix);
            light->setName(name);

            lightMap[name] = light;
        }
        break;
        }

        delete[] data;
    }

    fclose(dat);

    // ============================
    //  RICOSTRUZIONE GERARCHIA
    // ============================

    // TODO: OVO NON fornisce la gerarchia direttamente
    // → ma puoi salvarla durante il parsing
    // → oppure collegare mesh/light ai rispettivi nodi tramite nome

    // Per ora: tutto diventa CHILD della root
    for (auto& val : meshMap | views::values) root->addChildren(val);
    for (auto& val : lightMap | views::values) root->addChildren(val);
    for (auto& val : nodeMap | views::values)
        if (val != root)
            root->addChildren(val);

    return root;
}
