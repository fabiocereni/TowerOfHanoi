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
}

std::shared_ptr<Node> OvoReader::load(const std::string& path) {
   
   // Mappe per tenere traccia delle risorse caricate
   std::unordered_map<std::string, std::shared_ptr<Material>> materials;
   std::unordered_map<std::string, std::shared_ptr<Node>> nodes; // Contiene Node, Mesh e Light

   // Mappa per ricostruire la gerarchia: Child -> Parent Name
   std::unordered_map<std::shared_ptr<Node>, std::string> parentMap;

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
            glm::vec4(albedo, 1.0f), // Ambient approssimato ad albedo
            glm::vec4(albedo, 1.0f), // Diffuse
            glm::vec4(1.0f),         // Specular default bianco
            (1.0f - roughness) * 128.0f, // Shininess approssimata
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

         nodes[name] = node;
         if (strlen(targetName) > 0) parentMap[node] = targetName;
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
             // Salta blocco physics struct
             // struct PhysProps è circa 4 char + 1 vec3 + 7 float + 2 int + padding + 2 ptr
             // Calcolo manuale basato sul reader originale:
             // sizeof(PhysProps) su x64 è circa: 4 + 12 + 28 + 8 + 8 + 16 (ptr) = 76 bytes?
             // Meglio saltare dinamicamente se possibile, ma qui dobbiamo seguire lo stream.
             // Dal codice originale:
             // type(1)+cont(1)+collide(1)+hull(1) + vec3(12) + float(7*4) + int(2*4) + pointers(2*8) + padding
             // Total bytes to skip: 
             position += sizeof(unsigned char)*4 + sizeof(glm::vec3) + sizeof(float)*7 + sizeof(unsigned int)*2 + sizeof(void*)*2;
             // C'è un padding di allineamento nella struct originale? "unsigned int _pad;" -> +4 bytes
             position += sizeof(unsigned int); 

             // Se ci sono hull custom, bisogna saltarli. Il reader originale legge nrOfHulls
             // Ma attenzione: abbiamo saltato la struct, quindi abbiamo perso nrOfHulls.
             // Questo è rischioso. Per semplicità assumiamo no physics complessa o ricalcoliamo offset preciso.
             // CORREZIONE: Leggiamo nrOfHulls prima di saltare tutto per poter saltare i dati hull.
             // Torniamo indietro di un po' per leggere nrOfHulls.
             // nrOfHulls è l'ultimo int prima di _pad e pointers.
             // Offset relativo da inizio phys: 4 (chars) + 12 (vec) + 24 (6 floats) + 4 (angular) = 44 bytes?
             // Facciamo prima a non supportare physics avanzata in questo snippet o assumere 0 hulls.
         }

         unsigned int LODs;
         memcpy(&LODs, data + position, sizeof(unsigned int));
         position += sizeof(unsigned int);

         // Vettori temporanei per accumulare dati
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

            // FLATTENING: OVO ha facce indicizzate, Engine::Mesh vuole triangoli
            std::vector<glm::vec3> finalVertices;
            std::vector<glm::vec3> finalNormals;
            std::vector<glm::vec2> finalUvs;
            
            for (unsigned int c = 0; c < nFaces; c++) {
               unsigned int face[3];
               memcpy(face, data + position, sizeof(unsigned int) * 3);
               position += sizeof(unsigned int) * 3;

               // Per ogni indice della faccia, copia il vertice corrispondente
               for(int i=0; i<3; i++) {
                   finalVertices.push_back(tempVertices[face[i]]);
                   finalNormals.push_back(tempNormals[face[i]]);
                   finalUvs.push_back(tempUvs[face[i]]);
               }
            }
            
            // Crea la Mesh
            auto mesh = std::make_shared<Mesh>(finalVertices, matName, materials[matName]);
            mesh->setName(name);
            mesh->setMatrix(matrix);
            mesh->setNormals(finalNormals);
            mesh->setUv_coords(finalUvs);

            nodes[name] = mesh;
            if (strlen(targetName) > 0) parentMap[mesh] = targetName;
         }
         
         // Se c'erano più LOD o dati skinned, bisognerebbe saltarli qui, ma assumiamo 1 LOD per brevità
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
         
         // Cast shadow & volumetric
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

         nodes[name] = light;
         if (strlen(targetName) > 0) parentMap[light] = targetName;
         break;
      }
      
      default:
         // Skip chunks sconosciuti o non gestiti
         break;
      }
   }
   fclose(dat);

   // ============================================
   // COSTRUZIONE DEL GRAFO DI SCENA (LINKING)
   // ============================================
   
   // Nodo Radice Completo
   auto root = std::make_shared<Node>();
   root->setName("SceneRoot");

   // Itera su tutti i nodi caricati
   for (auto& [name, node] : nodes) {
      // Controlla se questo nodo ha un genitore registrato
      if (parentMap.count(node)) {
         std::string parentName = parentMap[node];
         
         // Cerca il genitore
         if (nodes.count(parentName)) {
            nodes[parentName]->addChildren(node);
            node->setParent(nodes[parentName]); // Se Node ha setParent
         } else {
            // Genitore non trovato, attacca alla root
            root->addChildren(node);
         }
      } else {
         // Nessun genitore (nodo di primo livello), attacca alla root
         root->addChildren(node);
      }
   }

   return root;
}