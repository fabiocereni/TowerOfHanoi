#include "engine.h"
#include "list.h"
#include "ovoreader.h"
#include "Test/test_suite.h"
#include "hanoiGame.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <algorithm>
#include <filesystem>


/* ----------------------
 * |     REMINDER       |
 * ----------------------
 * WORLD MATRIX:
 * Posizione, rotazione e scala dell'oggetto nello spazio globale.
 * Calcolata come: parentWorldMatrix * localMatrix.
 *
 * VIEW MATRIX:
 * È l'inversa della world matrix della camera.
 * Trasforma le coordinate dal mondo allo spazio della camera.
 *
 * MODELVIEW MATRIX:
 * Calcolata dalla render list: modelView = viewMatrix * worldMatrix.
 * Rappresenta l'oggetto nello spazio della camera (coordinate camera-space),
 * ed è quella che OpenGL usa per il rendering.
 */

namespace fs = std::filesystem;

int main(const int argc, char** argv) {
    // 1. Esegui i test
    TestSuite::runAllTests();

    // 2. Inizializza l'Engine
    Eng::Base& eng = Eng::Base::getInstance();
    eng.init(argc, argv, "Hanoi Tower Debug");



    fs::path sourceFile = __FILE__;

    fs::path projectDir = sourceFile.parent_path().parent_path();

    fs::path modelPath = projectDir / "ExportProgetto" / "ProvaTavoloovo.ovo";

    const std::string scenePath = modelPath.string();

    
    
    const std::shared_ptr<Eng::Texture> dark_wood_grain = eng.loadTextureFromFile((projectDir / "ExportProgetto" / "darkWood.dds").string());

    const std::shared_ptr<Eng::Texture> light_wood = eng.loadTextureFromFile((projectDir / "ExportProgetto" / "lightWood.dds").string());

    const std::shared_ptr<Eng::Texture> plastic = eng.loadTextureFromFile((projectDir / "ExportProgetto" / "plastic.dds").string());
   

    std::vector<std::shared_ptr<Eng::Texture>> textures;


    textures.push_back(plastic);
    textures.push_back(dark_wood_grain);
    textures.push_back(light_wood);
     

    // 3. Caricamento della Scena
    // Usa il nuovo metodo eng.load che restituisce la radice del grafo
    std::shared_ptr<Eng::Node> sceneRoot = eng.load(scenePath);


    for(auto var: sceneRoot->getChildrens())
    {
        if (var.get()->getName() == "[root]") {
            for (auto var1 : var->getChildrens())
            {
                std::cout << var1.get()->getName() << std::endl;
            }
        }
            
    }
   

    if (!sceneRoot) {
        std::cerr << "ERRORE: Impossibile caricare la scena da " << scenePath << std::endl;
        return -1;
    }

    eng.bindTexturesToMaterials(sceneRoot, textures);


    HanoiGame game(sceneRoot);

    eng.overrideKeyboardCallback([&](const unsigned char key, const int mouseX, const int mouseY) {

       switch (key) {
       case '1': // Nota gli apici ''
          game.processInput(1);
          break;
       case '2':
          game.processInput(2);
          break;
       case '3':
          game.processInput(3);
          break;
       case 27: // ESC
          std::cout << "Uscita..." << std::endl;
          // eng.close(); // se hai un metodo per chiudere
          break;
       default:
          // Ignora altri tasti
          break;
       }
       });



    // 4. Configurazione Telecamera
    // Far plane molto alto per vedere oggetti lontani
    const auto cam = eng.createPerspectiveCamera(60, 800.f / 600.f, 0.1f, 20000.0f);

    // --- POSIZIONAMENTO CON LOOKAT ---
    // Posizione: Alta (2000) e Indietro (2000)
    // Target: Centro (0,0,0)
    // Up: Asse Y (0,1,0)
    glm::vec3 position = glm::vec3(0.0f, 1400.0f, 400.0f);
    glm::vec3 target   = glm::vec3(0.0f, 1200.0f, 0.0f);
    glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 viewMatrix = glm::lookAt(position, target, up);

    // Invertiamo la view matrix perché l'engine si aspetta la World Matrix della camera
    cam->setMatrix(glm::inverse(viewMatrix));
    // ---------------------------------

    const auto renderList = std::make_shared<Eng::List>();

    // 5. Ciclo di Rendering
    while (eng.update()) {
        eng.clear();
        renderList->clear();

        // --- DEBUG SETTINGS ---
        // Attiva Wireframe per vedere la struttura dell'oggetto
        eng.setWireframe(false);
        // Disabilita luci per vedere il colore piatto/linee (utile se l'oggetto è nero/buio)
        eng.setLighting(true);
        // Disabilita culling per vedere anche l'interno o facce invertite
        eng.setCulling(true);
        // ----------------------

        // Carica matrice proiezione
        eng.begin3D(cam);

        // Passa la radice della scena alla render list
        renderList->pass(sceneRoot);

        // Esegui il render
        eng.render(cam, renderList);

        eng.end3D();

        // Reset opzionale delle impostazioni di debug (se volessi riabilitarle per la UI o altro)
        // eng.setWireframe(false);
        // eng.setLighting(true);
        // eng.setCulling(true);

        eng.showFps();
        eng.swap();
    }

    return 0;
}