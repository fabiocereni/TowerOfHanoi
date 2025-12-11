#pragma once

#include <memory>
#include <string>
#include <vector>
#include <list>
#include <filesystem>
#include <glm/glm.hpp>
#include <limits>

#include "engine.h"
#include "light.h"
#include "mesh.h"

/**
 * @class Setup
 * @brief Classe che gestisce l'inizializzazione della scena, le luci e le camere
 */
class Setup {

public:
    /// @brief Costruttore che inizializza i percorsi e i riferimenti base
    Setup(Eng::Base& eng, const std::string& projectDirName);

    /// @brief Carica la scena dal file OVO
    std::shared_ptr<Eng::Node> loadScene(const std::string& ovoFileName);

    /// @brief Inizializza la luce del lampadario
    std::shared_ptr<Eng::Light> initLampadario();

    /// @brief Crea la luce dinamica e il suo pivot
    std::shared_ptr<Eng::Node> createDynamicLight();

    /// @brief Crea e posiziona la camera frontale al tavolo
    std::shared_ptr<Eng::Camera> createFrontTableCamera();

    /// @brief Crea e posiziona la camera sopra il tavolo
    std::shared_ptr<Eng::Camera> createTopTableCamera();

    /// @brief Crea la camera dinamica libera
    std::shared_ptr<Eng::Camera> createDynamicCamera();

    /// @brief Calcola i limiti di movimento per la camera dinamica basandosi sulle mura
    void computeDynamicCameraLimits();

    /// @brief Calcola i limiti di altezza per l'animazione della luce dinamica
    void computeTreeHeightLimits();

    /// @brief Aggiorna la posizione e rotazione della camera dinamica
    void updateDynamicCameraMatrix(const std::shared_ptr<Eng::Camera>& cam);

    /// @brief Aggiorna l'animazione della luce dinamica (su/giù e rotazione)
    void updateDynamicLightAnimation();

    /// @brief Restituisce la lista dei nodi da escludere dal rendering (muri esterni per visuale interna, ecc.)
    const std::list<std::string>& getExcludedList() const { return excludedList_; }

    /// @brief Restituisce la root della scena
    std::shared_ptr<Eng::Node> getSceneRoot() const { return sceneRoot_; }

    /// @brief Restituisce la directory dell'export del progetto
    std::filesystem::path getProjectExportDir() const { return projectExportDir_; }

    // --- Metodi per interagire con lo stato interno dal Main (Input) ---

    /// @brief Muove la camera dinamica in avanti o indietro
    void moveDynamicCam(float delta);
    /// @brief Ruota la camera dinamica sull'asse X
    void rotateDynamicCamX(float delta);
    /// @brief Ruota la camera dinamica sull'asse Y
    void rotateDynamicCamY(float delta);
    /// @brief Muove la camera dinamica sull'asse Y (altezza)
    void liftDynamicCam(float delta);

    /// @brief Getter per la luce dinamica (per il toggle)
    std::shared_ptr<Eng::Light> getDynamicLight() const { return dynamicLight_; }

    /// @brief Getter per l'angolo X corrente della camera dinamica
    float getDynamicCamXAngle() const { return dynamicCamXAngle_; }
    /// @brief Getter per l'angolo Y corrente della camera dinamica
    float getDynamicCamYAngle() const { return dynamicCamYAngle_; }


private:
    /// @brief Calcola i vertici orientati nello spazio mondo
    void computeWorldOrientedVertexes(const std::vector<glm::vec3>& vertexes,
                                      const std::shared_ptr<Eng::Node>& node,
                                      std::vector<glm::vec3>& out) const;


    /// @brief Riferimento all'engine
    Eng::Base& eng_;

    /// @brief Root della scena caricata
    std::shared_ptr<Eng::Node> sceneRoot_;

    /// @brief Percorso base dell'export del progetto
    std::filesystem::path projectExportDir_;

    /// @brief Lista nodi esclusi dal render pass principale
    std::list<std::string> excludedList_;



   // Camera Dinamica

    /// @brief Posizione camera dinamica
    glm::vec3 dynamicCamPos_{0.0f, 800.0f, 1600.0f};
    /// @brief Angolo X camera dinamica
    float dynamicCamXAngle_ = 0.0f;
    /// @brief Angolo Y camera dinamica
    float dynamicCamYAngle_ = 0.0f;
    /// @brief Limiti fisici stanza (X min)
    float minX_ = std::numeric_limits<float>::max();
    /// @brief Limiti fisici stanza (X max)
    float maxX_ = std::numeric_limits<float>::lowest();
    /// @brief Limiti fisici stanza (Y min)
    float minY_ = std::numeric_limits<float>::max();
    /// @brief Limiti fisici stanza (Y max)
    float maxY_ = std::numeric_limits<float>::lowest();
    /// @brief Limiti fisici stanza (Z min)
    float minZ_ = std::numeric_limits<float>::max();
    /// @brief Limiti fisici stanza (Z max)
    float maxZ_ = std::numeric_limits<float>::lowest();


   // Luce Dinamica

    /// @brief Colore corrente luce dinamica
    glm::vec3 dynamicLightColor_{1.0f, 0.0f, 0.0f};
    /// @brief Posizione base del pivot luce
    glm::vec3 pivotBasePos_{0.0f};
    /// @brief Altezza corrente pivot luce
    float pivotY_ = 0.0f;
    /// @brief Angolo rotazione pivot luce
    float pivotAngle_ = 0.0f;
    /// @brief Direzione animazione luce (su/giù)
    bool lightGoingUp_ = true;
    /// @brief Limite inferiore altezza albero
    float minTreeY_ = std::numeric_limits<float>::max();
    /// @brief Limite superiore altezza albero
    float maxTreeY_ = std::numeric_limits<float>::lowest();

    /// @brief Puntatore alla luce dinamica
    std::shared_ptr<Eng::Light> dynamicLight_;
    /// @brief Puntatore al nodo pivot della luce
    std::shared_ptr<Eng::Node> lightPivotNode_;

};