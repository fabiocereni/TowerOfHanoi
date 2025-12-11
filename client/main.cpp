#include "engine.h"
#include "list.h"
#include "ovoreader.h"
#include "Test/test_suite.h"
#include "hanoiGame.h"
#include "setup.h"

int main(const int argc, char** argv) {

#ifdef DEBUG
    TestSuite::runAllTests();
#endif

    Eng::Base& eng = Eng::Base::getInstance();
    eng.init(argc, argv, "Hanoi Tower");

    /// @brief Istanzia la classe Setup specificando la cartella di export
    Setup setup(eng, "ExportProgetto");

    // @brief Carica la scena
    const auto sceneRoot = setup.loadScene("ExportFinalVersion.ovo");

    // @brief Setup texture
    eng.loadAndBindTextures(setup.getProjectExportDir(), sceneRoot);

    // @brief Inizializza il gioco
    HanoiGame game(sceneRoot);

    // @brief Crea le camere
    auto camFront = setup.createFrontTableCamera();
    auto camTop = setup.createTopTableCamera();
    auto camDynamic = setup.createDynamicCamera();

    // @brief Imposta camera iniziale
    eng.setActiveCamera(camFront);

    // @brief Calcola limiti per camera e luce
    setup.computeDynamicCameraLimits();
    setup.computeTreeHeightLimits();

    // @brief Posiziona inizialmente la camera dinamica
    setup.updateDynamicCameraMatrix(camDynamic);

    // @brief Setup Luci
    const auto luceLampadario = setup.initLampadario();
    auto pivotLuce = setup.createDynamicLight();

    // @brief Render list
    const auto renderList = std::make_shared<Eng::List>();


    /// @brief registrazione callbacks

    eng.overrideF1KeyBehaviour([&eng, &camFront] { eng.setActiveCamera(camFront); });
    eng.overrideF2KeyBehaviour([&eng, &camTop] { eng.setActiveCamera(camTop); });
    eng.overrideF3KeyBehaviour([&eng, &camDynamic] { eng.setActiveCamera(camDynamic); });


    constexpr float dynamic_cam_speed = 60.0f;

    eng.overrideUpArrowBehaviour([&] {
         setup.liftDynamicCam(dynamic_cam_speed);
         setup.updateDynamicCameraMatrix(camDynamic);
     });

    eng.overrideDownArrowBehaviour([&]{
        setup.liftDynamicCam(-dynamic_cam_speed);
        setup.updateDynamicCameraMatrix(camDynamic);
    });

    eng.overrideKeyboardCallback([&](const unsigned char key, const int mouseX, const int mouseY){

            constexpr float angle_speed = 2.0f;

            switch (key) {
            case '1': game.processInput(1); break;
            case '2': game.processInput(2); break;
            case '3': game.processInput(3); break;

            case 'w':
                setup.moveDynamicCam(dynamic_cam_speed);
                setup.updateDynamicCameraMatrix(camDynamic);
                break;
            case 's':
                setup.moveDynamicCam(-dynamic_cam_speed);
                setup.updateDynamicCameraMatrix(camDynamic);
                break;
            case 'a':
                setup.rotateDynamicCamX(angle_speed);
                setup.updateDynamicCameraMatrix(camDynamic);
                break;
            case 'd':
                setup.rotateDynamicCamX(-angle_speed);
                setup.updateDynamicCameraMatrix(camDynamic);
                break;
            case 'u':
                setup.rotateDynamicCamY(angle_speed);
                setup.updateDynamicCameraMatrix(camDynamic);
                break;
            case 'g':
                setup.rotateDynamicCamY(-angle_speed);
                setup.updateDynamicCameraMatrix(camDynamic);
                break;

            case 'x': game.undoMove(); break;
            case 'y': game.redoMove(); break;
            case 'r': game.resetGame(); break;
                
            case 'l':
                if(const auto dl = setup.getDynamicLight()) {
                    dl->toggleLight();
                    std::cout << "Dinamica: " << dl->isActive() << std::endl;
                }
                if(luceLampadario) {
                    luceLampadario->toggleLight();
                    std::cout << "Lampadario: " << luceLampadario->isActive() << std::endl;
                }
                break;
            default: break;
            }
        });




    while (eng.update()) {
        eng.clear();
        renderList->clear();

        eng.setWireframe(false);
        eng.setLighting(true);

        eng.begin3D(eng.getActiveCamera());

        renderList->pass(sceneRoot);

        eng.render(eng.getActiveCamera(), renderList, setup.getExcludedList());


        setup.updateDynamicLightAnimation();

        eng.end3D();

        eng.showFps();
        Eng::Base::infoPrinter(game.getCommandMessage());
        Eng::Base::statusPrinter(game.getStatusMessage());

        eng.swap();
    }

    eng.free();

    return 0;
}