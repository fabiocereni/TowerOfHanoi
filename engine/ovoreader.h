#pragma once

#include "engine_api.h"
#include "node.h" // Necessario per conoscere la classe Node
#include <string>
#include <memory>

namespace Eng {

    /**
     * Classe statica per il parsing e caricamento di file .ovo.
     */
    class ENG_API OvoReader {
    public:
        // La classe è puramente statica, non deve essere istanziata
        OvoReader() = delete;
        ~OvoReader() = delete;
        OvoReader(const OvoReader&) = delete;
        OvoReader& operator=(const OvoReader&) = delete;

        /**
         * @brief Carica una scena completa da un file .ovo.
         * * Legge il file, costruisce tutti i nodi (Mesh, Light, Materiali) e
         * ricostruisce la gerarchia padre-figlio.
         * * @param path Il percorso del file .ovo su disco.
         * @return std::shared_ptr<Node> Il nodo radice della scena caricata (o nullptr in caso di errore).
         */
        [[nodiscard]] static std::shared_ptr<Node> load(const std::string& path);

    };

}