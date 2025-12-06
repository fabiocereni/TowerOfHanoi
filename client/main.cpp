#include "engine.h"
#include "list.h"
#include "ovoreader.h"
#include "Test/test_suite.h"
#include "hanoiGame.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main(const int argc, char** argv) {
    // 1. Esegui i test

#ifdef DEBUG
    TestSuite::runAllTests();
#endif

    // 2. Inizializza l'Engine
    Eng::Base& eng = Eng::Base::getInstance();
    eng.init(argc, argv, "Hanoi Tower");


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


    eng.bindTexturesToMaterials(sceneRoot, textures);


    std::cout << "Number of lights: " << Eng::Light::getLightNumber() << std::endl;
    std::shared_ptr<Eng::OmnidirectionalLight> my_omnilight = eng.createOmnidirectionalLight();


    my_omnilight->setAmbient(glm::vec3(1.0f));
    my_omnilight->setDiffuse(glm::vec3(0.5f));
    my_omnilight->setSpecular(glm::vec3(0.5f));


    sceneRoot->addChildren(my_omnilight);

    std::cout << "Number of lights: " << Eng::Light::getLightNumber() << std::endl;

    HanoiGame game(sceneRoot);

    // 4. Configurazione Telecamera
    // Far plane molto alto per vedere oggetti lontani
    const auto cam = eng.createPerspectiveCamera(60, 800.f / 600.f, 0.1f, 20000.0f);

    // --- POSIZIONAMENTO CON LOOKAT ---
    // Posizione: Alta (2000) e Indietro (2000)
    // Target: Centro (0,0,0)
    // Up: Asse Y (0,1,0)
    auto position = glm::vec3(0.0f, 1400.0f, 400.0f);
    auto target   = glm::vec3(0.0f, 1200.0f, 0.0f);
    auto up       = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 viewMatrix = glm::lookAt(position, target, up);

    // Invertiamo la view matrix perché l'engine si aspetta la World Matrix della camera
    cam->setMatrix(glm::inverse(viewMatrix));
    // ---------------------------------

    const auto renderList = std::make_shared<Eng::List>();


    eng.overrideKeyboardCallback([&](const unsigned char key, const int mouseX, const int mouseY){
        switch (key)
        {
        case '1':
            game.processInput(1);
            break;
        case '2':
            game.processInput(2);
            break;
        case '3':
            game.processInput(3);
            break;
        default:
            // Ignora altri tasti
            break;
        }
    });

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

        eng.showFps();
        eng.infoPrinter(game.getStatusMessage());

        eng.swap();
    }


    return 0;
}