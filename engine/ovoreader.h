#pragma once

#include "engine_api.h"
#include "node.h"
#include <string>
#include <memory>

namespace Eng {

    namespace {
        /**
         * @class OvObject
         * @brief Definizioni dei tipi di entità nel file OVO
         */
        class OvObject {
        public:
            /**
             * @brief Enumerazione dei tipi di oggetti supportati
             */
            enum class Type : int {
                OBJECT = 0, NODE, OBJECT2D, OBJECT3D, LIST, BUFFER, SHADER, TEXTURE, FILTER, MATERIAL,
                FBO, QUAD, BOX, SKYBOX, FONT, CAMERA, LIGHT, BONE, MESH, SKINNED, INSTANCED, PIPELINE,
                EMITTER, ANIM, PHYSICS, LAST,
            };
        };

        /**
         * @class OvLight
         * @brief Sottotipi specifici per le luci
         */
        class OvLight {
        public:
            /**
             * @brief Enumerazione dei sottotipi di luce
             */
            enum class Subtype : int { OMNI = 0, DIRECTIONAL, SPOT, LAST };
        };

        /**
         * @struct NodeInfo
         * @brief Struttura per tenere traccia della gerarchia durante il parsing
         */
        struct NodeInfo {
            /** @brief puntatore al nodo corrente */
            std::shared_ptr<Node> node;
            /** @brief numero totale di figli attesi */
            unsigned int childrenCount;
            /** @brief numero di figli già processati */
            unsigned int childrenAdded;

            /**
             * @brief costruttore della struttura info nodo
             * @param n puntatore al nodo
             * @param count numero di figli
             */
            NodeInfo(const std::shared_ptr<Node>& n, const unsigned int count)
                : node(n), childrenCount(count), childrenAdded(0) {
            }
        };
    }

    /**
     * @class OvoReader
     * @brief Classe statica per il parsing e caricamento di file .ovo
     */
    class ENG_API OvoReader {
    public:
        /** @brief costruttore di default eliminato */
        OvoReader() = delete;
        /** @brief distruttore eliminato */
        ~OvoReader() = delete;
        /** @brief costruttore di copia eliminato */
        OvoReader(const OvoReader&) = delete;
        /** @brief operatore di assegnazione eliminato */
        OvoReader& operator=(const OvoReader&) = delete;

        /**
         * @brief Carica una scena completa da un file .ovo
         * @details Legge il file, costruisce tutti i nodi (Mesh, Light, Materiali) e
         * ricostruisce la gerarchia padre-figlio.
         * @param path Il percorso del file .ovo su disco.
         * @return Il nodo radice della scena caricata (o nullptr in caso di errore).
         */
        [[nodiscard]] static std::shared_ptr<Node> load(const std::string& path);

    };

}