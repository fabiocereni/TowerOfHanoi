#include "engine.h"
#include "list.h"
#include "ovoreader.h"
#include "Test/test_suite.h"
#include "hanoiGame.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <filesystem>

namespace fs = std::filesystem;


fs::path sourceFile = __FILE__;
fs::path projectDir = sourceFile.parent_path().parent_path();
fs::path modelPath = projectDir / "ExportProgetto" / "ProvaTavoloovoNew.ovo";

auto dynamic_cam_pos = glm::vec3(0.0f, 800.0f, 1600.0f);
float dynamic_cam_x_angle = 0.0f;
float dynamic_cam_y_angle = 0.0f;
float dynamic_cam_y = 800.0f;

std::shared_ptr<Eng::Camera> returnFrontTableCamera(Eng::Base& eng) {
    const auto frontTableCam = eng.createPerspectiveCamera(40, 1800.f / 1000.f, 0.1f, 20000.0f);


    constexpr auto frontTablePosition = glm::vec3(0.0f, 1600.0f, 400.0f);

    const glm::mat4 frontTableCamWorldMatrix = glm::translate(glm::mat4(1.0f), frontTablePosition) *
        glm::rotate(glm::mat4(1.0f), glm::radians(-40.0f), glm::vec3(1.0f, 0, 0));

    frontTableCam->setMatrix(frontTableCamWorldMatrix);

    return frontTableCam;
}

std::shared_ptr<Eng::Camera> returnTopTableCamera(Eng::Base& eng) {
    const auto topTableCam = eng.createPerspectiveCamera(40, 1800.f / 1000.f, 0.1f, 20000.0f);

    // posizione sopra al tavolo
    constexpr auto topTableCamPosition = glm::vec3(0.0f, 3000.0f, -700.0f);

    const glm::mat4 cam2WorldMatrix =
        glm::translate(glm::mat4(1.0f), topTableCamPosition) *
        glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));

    topTableCam->setMatrix(cam2WorldMatrix);
    return topTableCam;
}

void updateDynamicCamera(const std::shared_ptr<Eng::Camera>& cam) {

    dynamic_cam_pos.y = dynamic_cam_y;

    const glm::mat4 x_rot = glm::rotate(glm::mat4(1.0f),
                    glm::radians(dynamic_cam_x_angle),
                        glm::vec3(0, 1, 0));

    const glm::mat4 y_rot = glm::rotate(glm::mat4(1.0f),
                    glm::radians(dynamic_cam_y_angle),
                        glm::vec3(1, 0, 0));

    const glm::mat4 trans = glm::translate(glm::mat4(1.0f), dynamic_cam_pos);

    cam->setMatrix(trans * x_rot * y_rot);
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
    my_omnilight->setName("OmnidirectionalLight");

    sceneRoot->addChildren(my_omnilight);
}



int main(const int argc, char** argv) {

#ifdef DEBUG
    TestSuite::runAllTests();
#endif

    // Inizializza l'Engine
    Eng::Base& eng = Eng::Base::getInstance();
    eng.init(argc, argv, "Hanoi Tower");


    // Caricamento della Scena
    const std::string scenePath = modelPath.string();
    const auto sceneRoot = eng.load(scenePath);


    const std::vector<std::shared_ptr<Eng::Texture>> textures = loadAndReturnTextures(eng);
    eng.bindTexturesToMaterials(sceneRoot, textures);

    createAndReturnOmniDirectionalLight(eng, sceneRoot);

    HanoiGame game(sceneRoot);


    eng.setActiveCamera(returnFrontTableCamera(eng));

    const auto renderList = std::make_shared<Eng::List>();

    auto cam3 = eng.createPerspectiveCamera(40, 1800.f / 1000.f, 0.1f, 20000.0f);

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
        case 'w':
            dynamic_cam_pos.z -= 15.0f;
            updateDynamicCamera(cam3);
            break;
        case 's':
            dynamic_cam_pos.z += 15.0f;
            updateDynamicCamera(cam3);
            break;
        case 'a':
            dynamic_cam_x_angle += 0.5f;
            updateDynamicCamera(cam3);
            break;
        case 'd':
            dynamic_cam_x_angle -= 0.5f;
            updateDynamicCamera(cam3);
            break;

        case 'u':
            dynamic_cam_y_angle += 0.5f;
            updateDynamicCamera(cam3);
            break;

        case 'g':
            dynamic_cam_y_angle -= 0.5f;
            updateDynamicCamera(cam3);
            break;

        default:
            // Ignora altri tasti
            break;
        }
    });

    auto cam1 = returnFrontTableCamera(eng);
    eng.overrideF1KeyBehaviour([&eng, &cam1] {eng.setActiveCamera(cam1);});

    auto cam2 = returnTopTableCamera(eng);
    eng.overrideF2KeyBehaviour([&cam2, &eng] {eng.setActiveCamera(cam2);});

    updateDynamicCamera(cam3);

    eng.overrideF3KeyBehaviour([&cam3, &eng] {eng.setActiveCamera(cam3);});

    eng.overrideUpArrowBehaviour([&] {
        dynamic_cam_y += 15.0f;
        updateDynamicCamera(cam3);
    });


    eng.overrideDownArrowBehaviour([&]{
        dynamic_cam_y -= 15.0f;
        updateDynamicCamera(cam3);
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
        eng.begin3D(eng.getActiveCamera());

        // Passa la radice della scena alla render list
        renderList->pass(sceneRoot);

        // Esegui il render
        eng.render(eng.getActiveCamera(), renderList);

        eng.end3D();

        eng.showFps();
        Eng::Base::infoPrinter(game.getStatusMessage());

        eng.swap();
    }
    return 0;
}