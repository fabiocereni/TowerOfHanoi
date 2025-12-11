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

fs::path sourceFile = __FILE__;
fs::path projectDir = sourceFile.parent_path().parent_path();
fs::path modelPath = projectDir / "ExportProgetto" / "ProvaTavoloovoNew.ovo";

auto dynamic_cam_pos = glm::vec3(0.0f, 800.0f, 1600.0f);
float dynamic_cam_x_angle = 0.0f;
float dynamic_cam_y_angle = 0.0f;
float dynamic_cam_y = 800.0f;

float angle_speed_rotation = 2.0f;
float dynamic_cam_speed = 60.0f;

// CAMERA LIMITS
float minX = std::numeric_limits<float>::max();
float maxX = std::numeric_limits<float>::lowest();
float minY = std::numeric_limits<float>::max();
float maxY = std::numeric_limits<float>::lowest();
float minZ = std::numeric_limits<float>::max();
float maxZ = std::numeric_limits<float>::lowest();



std::shared_ptr<Eng::Node> findRec(std::shared_ptr<Eng::Node> start, const std::string& nameToFind) {
    if (!start) return nullptr;


    if (start->getName() == nameToFind) {
        return start;
    }


    for (const auto& child : start->getChildren()) {
        auto result = findRec(child, nameToFind);
        if (result != nullptr) {
            return result; // Trovato! Risaliamo la catena
        }
    }

    return nullptr;
}


std::shared_ptr<Eng::Light> initLampadario(const std::shared_ptr<Eng::Node>& sceneRoot, Eng::Base& eng) {
    const std::shared_ptr<Eng::Mesh> lampadarioNode = std::dynamic_pointer_cast<Eng::Mesh>(findRec(sceneRoot, "lampadario"));

    if (!lampadarioNode) {
        std::cerr << ">> ATTENZIONE: Nodo '" << "nomeNodoLampadario" << "' non trovato. Luce non creata." << std::endl;
        return nullptr;
    }


    const auto lampadarioLight = eng.createOmnidirectionalLight();
    lampadarioLight->setAmbient(glm::vec3(4.5f));
    lampadarioLight->setDiffuse(glm::vec3(4.5f));
    lampadarioLight->setSpecular(glm::vec3(4.5f));

    lampadarioLight->setRadius(3000.0f);

    // 5. Appendi la luce al nodo lampadario
    lampadarioNode->addChildren(lampadarioLight);
    lampadarioLight->setName("lampadario_light");

    std::cout << ">> Luce omnidirezionale creata e attaccata a: " << "nomeNodoLampadario" << std::endl;

    lampadarioNode->getMaterial()->setEmission(glm::vec4(1.0f));

    return lampadarioLight;
}


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
    constexpr float limitation_bound = 500.0f;

    computeDynamicCameraLimits(sceneRoot);

    dynamic_cam_pos.x = glm::clamp(dynamic_cam_pos.x, minX + limitation_bound, maxX - limitation_bound);
    dynamic_cam_pos.y = glm::clamp(dynamic_cam_pos.y, minY + limitation_bound, maxY - limitation_bound);
    dynamic_cam_pos.z = glm::clamp(dynamic_cam_pos.z, minZ + limitation_bound, maxZ - limitation_bound);

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


    const fs::path textureDir = projectDir / "ExportProgetto";

    if (!fs::exists(textureDir) || !fs::is_directory(textureDir)) {
        std::cerr << "ATTENZIONE: La cartella texture non esiste: " << textureDir << std::endl;
        return textures;
    }

    std::cout << "--- CARICAMENTO TEXTURE AUTOMATICO ---" << std::endl;

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

auto colore_luce = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 pivot_base_pos(0.0f);
float pivot_y = 0.0f;
float pivot_angle = 0.0f;
std::shared_ptr<Eng::Light> dynamic_light;

std::shared_ptr<Eng::Node> createDynamicLight(const std::shared_ptr<Eng::Node>& sceneRoot) {

    const auto luce = Eng::OmnidirectionalLight::createOmnidirectionalLight();
    luce->setName("dynamic_light");

    luce->setAmbient(colore_luce);
    luce->setDiffuse(colore_luce);
    luce->setSpecular(colore_luce);
    luce->setRadius(1000.0f);

    const auto albero = std::dynamic_pointer_cast<Eng::Mesh>(findRec(sceneRoot, "albero"));

    const auto pivot = std::make_shared<Eng::Node>();
    pivot->setName("pivot");

    if (albero) {
        const auto posAlbero = glm::vec3(albero->getWorldMatrix()[3]);

        pivot_base_pos = posAlbero;
        pivot_y = posAlbero.y;
        pivot_angle = 0.0f;

        pivot->setMatrix(glm::mat4(1.0f));
        sceneRoot->addChildren(pivot);

        luce->setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(230.0f, 0.0f, 0.0f)));
        pivot->addChildren(luce);
        luce->setParent(pivot);
    }

    dynamic_light = luce;
    dynamic_light->setActive(false);

    return pivot;
}


void computeTreeHeighLimits(float& minY_tree,
                            float& maxY_tree,
                            const std::shared_ptr<Eng::Node>& sceneRoot) {

    minY_tree = std::numeric_limits<float>::max();
    maxY_tree = std::numeric_limits<float>::lowest();

    const auto albero = findRec(sceneRoot, "albero");
    const auto cone0 = std::dynamic_pointer_cast<Eng::Mesh>(findRec(sceneRoot, "Cone"));
    const auto cone1 = std::dynamic_pointer_cast<Eng::Mesh>(findRec(sceneRoot, "Cone_001"));
    const auto cone2 = std::dynamic_pointer_cast<Eng::Mesh>(findRec(sceneRoot, "Cone_002"));


    if (!albero || (!cone0 && !cone2)) {
        std::cerr << "ERRORE: Nodo albero o coni non trovati per i limiti." << std::endl;
        minY_tree = maxY_tree = pivot_y;
        return;
    }


    auto processMesh = [&](const std::shared_ptr<Eng::Mesh>& mesh) {
        const auto& verts = mesh->getVertexes();


        glm::mat4 globalTransform = albero->getMatrix() * mesh->getMatrix();

        for (const auto& v : verts) {
            glm::vec4 worldV = globalTransform * glm::vec4(v, 1.0f);

            if (worldV.y < minY_tree)
                minY_tree = worldV.y;

            if (worldV.y > maxY_tree)
                maxY_tree = worldV.y;
        }
    };


    if (cone0)
        processMesh(cone0);

    if (cone2)
        processMesh(cone2);


    if (minY_tree == std::numeric_limits<float>::max()) {
         minY_tree = maxY_tree = pivot_y;
    }

}



void updateDynamicLight(const std::shared_ptr<Eng::Node>& pivot,
                        const float minY_tree,
                        const float maxY_tree) {
    if (!pivot)
        return;

    static bool goingUp = true;
    constexpr float intensity = 5.0f;
    const float bottom = minY_tree;
    const float top = maxY_tree;
    constexpr float speed = 4.0f;

    if (goingUp) {
        pivot_y += speed;
        colore_luce = glm::vec3(0.0f, intensity, 0.0f);
        dynamic_light->setAmbient(colore_luce);
        dynamic_light->setDiffuse(colore_luce);
        dynamic_light->setSpecular(colore_luce);
    }
    else {
        pivot_y -= speed;
        colore_luce = glm::vec3(intensity, 0.0f, 0.0f);
        dynamic_light->setAmbient(colore_luce);
        dynamic_light->setDiffuse(colore_luce);
        dynamic_light->setSpecular(colore_luce);
    }


    if (pivot_y >= top) {
        pivot_y = top;
        goingUp = false;
    } else if (pivot_y <= bottom) {
        pivot_y = bottom;
        goingUp = true;
    }

    pivot_angle += 5.0f;

    glm::mat4 m(1.0f);
    m = glm::translate(m, glm::vec3(pivot_base_pos.x, pivot_y, pivot_base_pos.z));
    m = m * glm::rotate(glm::mat4(1.0f), glm::radians(pivot_angle), glm::vec3(0, 1, 0));
    pivot->setMatrix(m);
}










int main(const int argc, char** argv) {

#ifdef DEBUG
    TestSuite::runAllTests();
#endif


    Eng::Base& eng = Eng::Base::getInstance();
    eng.init(argc, argv, "Hanoi Tower");


    const std::string scenePath = modelPath.string();
    const auto sceneRoot = eng.load(scenePath);

    const std::list<std::string> excluded = {
        "muro1", "muro2", "muro3", "muro4", "soffitto", "pavimento" ,"lampadario"
    };





    const std::vector<std::shared_ptr<Eng::Texture>> textures = loadAndReturnTextures(eng);
    eng.bindTexturesToMaterials(sceneRoot, textures);

    HanoiGame game(sceneRoot);

    eng.setActiveCamera(returnFrontTableCamera(eng));

    const auto renderList = std::make_shared<Eng::List>();

    auto cam3 = eng.createPerspectiveCamera(80, 1800.f / 1000.f, 10.0f, 20000.0f);



    auto cam1 = returnFrontTableCamera(eng);
    eng.overrideF1KeyBehaviour([&eng, &cam1] {eng.setActiveCamera(cam1);});

    auto cam2 = returnTopTableCamera(eng);
    eng.overrideF2KeyBehaviour([&cam2, &eng] {eng.setActiveCamera(cam2);});

    updateDynamicCamera(cam3, sceneRoot);

    auto pivot = std::make_shared<Eng::Node>();




    eng.overrideF3KeyBehaviour([&cam3, &eng] {eng.setActiveCamera(cam3);});

    eng.overrideUpArrowBehaviour([&] {
         dynamic_cam_pos.y += dynamic_cam_speed;
         updateDynamicCamera(cam3, sceneRoot);
     });

    eng.overrideDownArrowBehaviour([&]{
        dynamic_cam_pos.y -= dynamic_cam_speed;
        updateDynamicCamera(cam3, sceneRoot);
    });

    float minY_tree = std::numeric_limits<float>::max();
    float maxY_tree = std::numeric_limits<float>::lowest();


    computeTreeHeighLimits(minY_tree, maxY_tree, sceneRoot);


    auto luce_lampadario = initLampadario(sceneRoot, eng);
    pivot = createDynamicLight(sceneRoot);



    eng.overrideKeyboardCallback([&](const unsigned char key, const int mouseX, const int mouseY){

          // estraendo z dalla matrice possiamo capire dove stiamo guardando
            const glm::mat4 orientation = glm::rotate(glm::mat4(1.0f), glm::radians(dynamic_cam_x_angle), glm::vec3(0, 1, 0)) *
                                          glm::rotate(glm::mat4(1.0f), glm::radians(dynamic_cam_y_angle), glm::vec3(1, 0, 0));


            const glm::vec3 forwardDir = -glm::vec3(orientation[2]);

            switch (key) {
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
                dynamic_cam_pos += forwardDir * dynamic_cam_speed;
                updateDynamicCamera(cam3, sceneRoot);
                break;
            case 's':
                dynamic_cam_pos -= forwardDir * dynamic_cam_speed;
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

            case 'x':
                game.undoMove();
                 break;

             case 'y':
                game.redoMove();
                break;

             case 'r':
                game.resetGame();
                break;

            case 'j':
                dynamic_light->toggleLight();
                std::cout << "Dinamica: " << dynamic_light->isActive() << std::endl;
                luce_lampadario->toggleLight();
                std::cout << "Lampadario: " << luce_lampadario->isActive() << std::endl;
             default:
                break;
            }
        });




    while (eng.update()) {
        eng.clear();
        renderList->clear();


        eng.setWireframe(false);
        eng.setLighting(true);


        eng.begin3D(eng.getActiveCamera());


        renderList->pass(sceneRoot);


        eng.render(eng.getActiveCamera(), renderList, excluded);


        updateDynamicLight(pivot, minY_tree, maxY_tree);




        eng.end3D();

        eng.showFps();
        Eng::Base::infoPrinter(game.getStatusMessage());

        eng.swap();
    }

    eng.free();
    return 0;
}