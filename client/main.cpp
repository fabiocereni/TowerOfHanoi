#include "engine.h"
#include "list.h"
#include "ovoreader.h"
#include "Test/test_suite.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

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

int main(const int argc, char** argv) {
    // 1. Esegui i test
    TestSuite::runAllTests();

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

    // Imposta sfondo grigio per vedere meglio il wireframe
    eng.changeBackgroundColor(0.3f, 0.3f, 0.3f);

    const std::string scenePath = "\\Users\\matti\\Documents\\Uni\\grafica\\Progetto\\Export3DSMAX Progetto\\ProvaTavoloovo.ovo";

    // 3. Caricamento della Scena
    // Usa il nuovo metodo eng.load che restituisce la radice del grafo
    std::shared_ptr<Eng::Node> sceneRoot = eng.load(scenePath);

    for(auto var: sceneRoot->getChildrens())
    {
        if (var.get()->getName() == "baseHanoi") {
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
