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

fs::path sourceFile = __FILE__;
fs::path projectDir = sourceFile.parent_path().parent_path();
fs::path modelPath = projectDir / "ExportProgetto" / "ProvaTavoloovoNew.ovo";



std::shared_ptr<Eng::Camera> returnFrontTableCamera(Eng::Base& eng) {
    const auto frontTableCam = eng.createPerspectiveCamera(40, 800.f / 600.f, 0.1f, 20000.0f);


    constexpr auto frontTablePosition = glm::vec3(0.0f, 1600.0f, 400.0f);

    const glm::mat4 frontTableCamWorldMatrix = glm::translate(glm::mat4(1.0f), frontTablePosition) *
        glm::rotate(glm::mat4(1.0f), glm::radians(-40.0f), glm::vec3(1.0f, 0, 0));

    frontTableCam->setMatrix(frontTableCamWorldMatrix);

    return frontTableCam;
}

std::shared_ptr<Eng::Camera> returnTopTableCamera(Eng::Base& eng) {
    const auto cam2 = eng.createPerspectiveCamera(40, 800.f / 600.f, 0.1f, 20000.0f);

    // posizione sopra al tavolo
    constexpr auto cam2Pos = glm::vec3(0.0f, 3000.0f, -700.0f);

    const glm::mat4 cam2WorldMatrix =
        glm::translate(glm::mat4(1.0f), cam2Pos) *
        glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));

    cam2->setMatrix(cam2WorldMatrix);
    return cam2;
}

std::vector<std::shared_ptr<Eng::Texture>> loadAndReturnTextures(const Eng::Base& eng) {


    const std::shared_ptr<Eng::Texture> dark_wood_grain = eng.loadTextureFromFile((projectDir / "ExportProgetto" / "darkWood.dds").string());

    const std::shared_ptr<Eng::Texture> light_wood = eng.loadTextureFromFile((projectDir / "ExportProgetto" / "lightWood.dds").string());

    const std::shared_ptr<Eng::Texture> plastic = eng.loadTextureFromFile((projectDir / "ExportProgetto" / "plastic.dds").string());


    std::vector<std::shared_ptr<Eng::Texture>> textures;

    textures.push_back(plastic);
    textures.push_back(dark_wood_grain);
    textures.push_back(light_wood);

    return textures;
}

void createAndReturnOmniDirectionalLight(Eng::Base& eng, const std::shared_ptr<Eng::Node>& sceneRoot) {
    std::shared_ptr<Eng::OmnidirectionalLight> my_omnilight = eng.createOmnidirectionalLight();

    my_omnilight->setAmbient(glm::vec3(1.0f));
    my_omnilight->setDiffuse(glm::vec3(0.5f));
    my_omnilight->setSpecular(glm::vec3(0.5f));


    sceneRoot->addChildren(my_omnilight);
}

int main(const int argc, char** argv) {
    // 1. Esegui i test

#ifdef DEBUG
    TestSuite::runAllTests();
#endif

    // 2. Inizializza l'Engine
    Eng::Base& eng = Eng::Base::getInstance();
    eng.init(argc, argv, "Hanoi Tower");


    // 3. Caricamento della Scena
    const std::string scenePath = modelPath.string();
    std::shared_ptr<Eng::Node> sceneRoot = eng.load(scenePath);


    const std::vector<std::shared_ptr<Eng::Texture>> textures = loadAndReturnTextures(eng);
    eng.bindTexturesToMaterials(sceneRoot, textures);

    createAndReturnOmniDirectionalLight(eng, sceneRoot);

    HanoiGame game(sceneRoot);



    // ---------------------------------

    eng.setActiveCamera(returnFrontTableCamera(eng));

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

    auto cam1 = returnFrontTableCamera(eng);
    eng.overrideF1KeyBehaviour([&eng, &cam1]() {eng.setActiveCamera(cam1);});

    auto cam2 = returnTopTableCamera(eng);
    eng.overrideF2KeyBehaviour([&cam2, &eng](){eng.setActiveCamera(cam2);});

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
        eng.begin3D(eng.getActiveCamera());

        // Passa la radice della scena alla render list
        renderList->pass(sceneRoot);

        // Esegui il render
        eng.render(eng.getActiveCamera(), renderList);

        eng.end3D();

        eng.showFps();
        eng.infoPrinter(game.getStatusMessage());

        eng.swap();
    }


    return 0;
}