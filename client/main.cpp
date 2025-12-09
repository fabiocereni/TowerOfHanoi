#include <algorithm>

#include "engine.h"
#include "list.h"
#include "ovoreader.h"
#include "Test/test_suite.h"
#include "hanoiGame.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <filesystem>

namespace fs = std::filesystem;

<<<<<<< HEAD
int main(const int argc, char** argv) {
    // 1. Esegui i test
    //TestSuite::runAllTests();

    // 2. Inizializza l'Engine
    Eng::Base& eng = Eng::Base::getInstance();
    eng.overrideKeyboardCallback([](const unsigned char key, const int mouseX, const int mouseY){

        std::cout << key << std::endl;

        switch (key) {
       // da implementare
        }
    });

    eng.overrideUpArrowBehaviour([] {std::cout << "UP ARROW" << std::endl;});
    eng.overrideDownArrowBehaviour([]{std::cout << "DOWN ARROW" << std::endl;});
    eng.overrideRightArrowBehaviour([]{std::cout << "RIGHT ARROW" << std::endl;});
    eng.overrideLeftArrowBehaviour([]{std::cout << "LEFT ARROW" << std::endl;});


    eng.init(argc, argv, "Hanoi Tower Debug");
=======
fs::path sourceFile = __FILE__;
fs::path projectDir = sourceFile.parent_path().parent_path();
fs::path modelPath = projectDir / "ExportProgetto" / "ProvaTavoloovoNew.ovo";

auto dynamic_cam_pos = glm::vec3(0.0f, 800.0f, 1600.0f);
float dynamic_cam_x_angle = 0.0f;
float dynamic_cam_y_angle = 0.0f;
float dynamic_cam_y = 800.0f;
>>>>>>> origin/dynamicCameraMerge

float angle_speed_rotation = 2.0f;
float dynamic_cam_speed = 60.0f;

<<<<<<< HEAD
    const std::string scenePath = "C:\\ProvaTavoloovoNew.ovo";
=======
// CAMERA LIMITS
float minX = std::numeric_limits<float>::max();
float maxX = std::numeric_limits<float>::lowest();
float minY = std::numeric_limits<float>::max();
float maxY = std::numeric_limits<float>::lowest();
float minZ = std::numeric_limits<float>::max();
float maxZ = std::numeric_limits<float>::lowest();
>>>>>>> origin/dynamicCameraMerge


std::shared_ptr<Eng::Camera> returnFrontTableCamera(Eng::Base& eng) {
    const auto frontTableCam = eng.createPerspectiveCamera(40, 800.f / 600.f, 10.0f, 20000.0f);


    constexpr auto frontTablePosition = glm::vec3(0.0f, 2000.0f, 700.0f);

    const glm::mat4 frontTableCamWorldMatrix = glm::translate(glm::mat4(1.0f), frontTablePosition) *
        glm::rotate(glm::mat4(1.0f), glm::radians(-40.0f), glm::vec3(1.0f, 0, 0));

    frontTableCam->setMatrix(frontTableCamWorldMatrix);

    return frontTableCam;
}

std::shared_ptr<Eng::Camera> returnTopTableCamera(Eng::Base& eng) {
    const auto topTableCam = eng.createPerspectiveCamera(40, 800.f / 600.f, 10.0f, 20000.0f);

    // posizione sopra al tavolo
    constexpr auto topTableCamPosition = glm::vec3(0.0f, 3000.0f, -700.0f);

    const glm::mat4 cam2WorldMatrix =
        glm::translate(glm::mat4(1.0f), topTableCamPosition) *
        glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));

    topTableCam->setMatrix(cam2WorldMatrix);
    return topTableCam;
}

std::shared_ptr<Eng::Node> findRec(std::shared_ptr<Eng::Node> current, const std::string& nameToFind) {
    if (!current) return nullptr;

    // 1. Controlla se e' questo
    if (current->getName() == nameToFind) {
        return current;
    }

    // 2. Cerca nei figli
    for (const auto& child : current->getChildren()) {
        auto result = findRec(child, nameToFind);
        if (result != nullptr) {
            return result; // Trovato! Risaliamo la catena
        }
    }

    // 3. Non trovato qui sotto
    return nullptr;
}

void computeWorldOrientedVertexes(const std::vector<glm::vec3>& vertexes,
                                  const std::shared_ptr<Eng::Node>& node,
                                  std::vector<glm::vec3>& out) {
    const glm::mat4 world = node->getWorldMatrix();

    for (const auto& v : vertexes) {
        out.push_back(glm::vec3(world * glm::vec4(v, 1.0f)));
    }
}

void computeDynamicCameraLimits(const std::shared_ptr<Eng::Node>& sceneRoot) {

    std::vector<glm::vec3> room_vertexes;

    const auto floor = findRec(sceneRoot, "pavimento");
    const std::vector<glm::vec3> floor_vec = std::dynamic_pointer_cast<Eng::Mesh>(floor)->getVertexes();
    computeWorldOrientedVertexes(floor_vec, floor, room_vertexes);


    const auto wall_1 = findRec(sceneRoot, "muro1");
    const auto wall_1_vec = std::dynamic_pointer_cast<Eng::Mesh>(wall_1)->getVertexes();
    computeWorldOrientedVertexes(wall_1_vec, wall_1, room_vertexes);


    const auto wall_2 = findRec(sceneRoot, "muro2");
    const auto wall_2_vec = std::dynamic_pointer_cast<Eng::Mesh>(wall_2)->getVertexes();
    computeWorldOrientedVertexes(wall_2_vec, wall_2, room_vertexes);


    const auto wall_3 = findRec(sceneRoot, "muro3");
    const auto wall_3_vec = std::dynamic_pointer_cast<Eng::Mesh>(wall_3)->getVertexes();
    computeWorldOrientedVertexes(wall_3_vec, wall_3, room_vertexes);


    const auto wall_4 = findRec(sceneRoot, "muro4");
    const auto wall_4_vec = std::dynamic_pointer_cast<Eng::Mesh>(wall_4)->getVertexes();
    computeWorldOrientedVertexes(wall_4_vec, wall_4, room_vertexes);


    for (auto& v : room_vertexes) {
        minX = std::min(minX, v.x);
        maxX = std::max(maxX, v.x);
        minY = std::min(minY, v.y);
        maxY = std::max(maxY, v.y);
        minZ = std::min(minZ, v.z);
        maxZ = std::max(maxZ, v.z);
    }

}


void updateDynamicCamera(const std::shared_ptr<Eng::Camera>& cam, const std::shared_ptr<Eng::Node>& sceneRoot) {
    constexpr float limitation_bound = 100.0f;


    computeDynamicCameraLimits(sceneRoot);

    dynamic_cam_pos.y = dynamic_cam_y;


    dynamic_cam_pos.x = glm::clamp(dynamic_cam_pos.x, minX+limitation_bound, maxX-limitation_bound);
    dynamic_cam_pos.y = glm::clamp(dynamic_cam_pos.y, minY+limitation_bound, maxY-limitation_bound);
    dynamic_cam_pos.z = glm::clamp(dynamic_cam_pos.z, minZ+limitation_bound, maxZ-limitation_bound);


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
    std::vector<std::shared_ptr<Eng::Texture>> textures;

    // Definisci il percorso della cartella texture
    // Nota: Basandomi sui tuoi file precedenti, le texture erano in "ExportProgetto".
    // Se hai creato una cartella specifica chiamata "texture", cambia "ExportProgetto" in "texture".
    fs::path textureDir = projectDir / "ExportProgetto";

    // Verifica se la cartella esiste
    if (!fs::exists(textureDir) || !fs::is_directory(textureDir)) {
        std::cerr << "ATTENZIONE: La cartella texture non esiste: " << textureDir << std::endl;
        return textures;
    }

    std::cout << "--- CARICAMENTO TEXTURE AUTOMATICO ---" << std::endl;

    // Itera su tutti i file nella cartella
    for (const auto& entry : fs::directory_iterator(textureDir)) {
        if (entry.is_regular_file()) {
            // Ottieni l'estensione del file
            std::string ext = entry.path().extension().string();

            // Converti in minuscolo per il confronto (per gestire .PNG, .png, .DDS, ecc.)
            std::transform(ext.begin(), ext.end(), ext.begin(),
                [](unsigned char c) { return std::tolower(c); });

            // Lista delle estensioni supportate (aggiungine altre se serve)
            if (ext == ".dds" || ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".tga" || ext == ".bmp") {

                std::cout << "Loading: " << entry.path().filename() << "... ";

                // Carica la texture usando l'engine
                auto tex = eng.loadTextureFromFile(entry.path().string());

                if (tex) {
                    textures.push_back(tex);
                    std::cout << "OK" << std::endl;
                }
                else {
                    std::cout << "FALLITO" << std::endl;
                }
            }
        }
    }
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Totale texture caricate: " << textures.size() << std::endl;

    return textures;
}

void createAndReturnOmniDirectionalLight(Eng::Base& eng, const std::shared_ptr<Eng::Node>& sceneRoot) {
    const std::shared_ptr<Eng::OmnidirectionalLight> my_omnilight = eng.createOmnidirectionalLight();

    my_omnilight->setAmbient(glm::vec3(5.0f));
    my_omnilight->setDiffuse(glm::vec3(5.0f));
    my_omnilight->setSpecular(glm::vec3(5.0f));
    my_omnilight->setRadius(3000.0f);

    constexpr glm::mat4 lightMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2000.0f, 0.0f));
    my_omnilight->setMatrix(lightMatrix);

    my_omnilight->setName("luce1");

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

    std::list<std::string> excluded = { "muro1", "muro2", "muro3", "muro4", "soffitto", "pavimento" ,"lampadario"};

    const std::vector<std::shared_ptr<Eng::Texture>> textures = loadAndReturnTextures(eng);
    eng.bindTexturesToMaterials(sceneRoot, textures);

    createAndReturnOmniDirectionalLight(eng, sceneRoot);

    HanoiGame game(sceneRoot);

    eng.setActiveCamera(returnFrontTableCamera(eng));

    const auto renderList = std::make_shared<Eng::List>();

    auto cam3 = eng.createPerspectiveCamera(40, 1800.f / 1000.f, 10.0f, 20000.0f);

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
            dynamic_cam_pos.z -= dynamic_cam_speed;
            updateDynamicCamera(cam3, sceneRoot);
            break;
        case 's':
            dynamic_cam_pos.z += dynamic_cam_speed;
            updateDynamicCamera(cam3, sceneRoot);
            break;
        case 'a':
            dynamic_cam_x_angle += angle_speed_rotation;
            updateDynamicCamera(cam3, sceneRoot);
            break;
        case 'd':
            dynamic_cam_x_angle -= angle_speed_rotation;
            updateDynamicCamera(cam3, sceneRoot);
            break;

        case 'u':
            dynamic_cam_y_angle += angle_speed_rotation;
            updateDynamicCamera(cam3, sceneRoot);
            break;

        case 'g':
            dynamic_cam_y_angle -= angle_speed_rotation;
            updateDynamicCamera(cam3, sceneRoot);
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

    updateDynamicCamera(cam3, sceneRoot);

    eng.overrideF3KeyBehaviour([&cam3, &eng] {eng.setActiveCamera(cam3);});

    eng.overrideUpArrowBehaviour([&] {
        dynamic_cam_y += dynamic_cam_speed;
        updateDynamicCamera(cam3, sceneRoot);
    });

    eng.overrideDownArrowBehaviour([&]{
        dynamic_cam_y -= dynamic_cam_speed;
        updateDynamicCamera(cam3, sceneRoot);
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

        // Carica matrice proiezione
        eng.begin3D(eng.getActiveCamera());

        // Passa la radice della scena alla render list
        renderList->pass(sceneRoot);

        // Esegui il render
        eng.render(eng.getActiveCamera(), renderList, excluded);

        eng.end3D();

        eng.showFps();
        Eng::Base::infoPrinter(game.getStatusMessage());

        eng.swap();
    }
    return 0;
}
