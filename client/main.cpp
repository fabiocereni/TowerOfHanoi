#include "engine.h"
#include "list.h"
#include "Test/test_suite.h"


/*                     ----------------------
 *                     |     REMINDER       |
 *                     ----------------------
 * WORLD MATRIX:
 *     Posizione, rotazione e scala dell'oggetto nello spazio globale.
 *     Calcolata come: parentWorldMatrix * localMatrix.
 *
 * VIEW MATRIX:
 *     È l'inversa della world matrix della camera.
 *     Trasforma le coordinate dal mondo allo spazio della camera.
 *
 * MODELVIEW MATRIX:
 *     Calcolata dalla render list: modelView = viewMatrix * worldMatrix.
 *     Rappresenta l'oggetto nello spazio della camera (coordinate camera-space),
 *     ed è quella che OpenGL usa per il rendering.
 */

int main(const int argc, char** argv) {

    // esegue tutti i test, se passano il programma continua
    TestSuite::runAllTests();


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


    eng.init(argc, argv, "Hanoi Tower");

    const auto root = eng.load("\\Mac\Home\Downloads\ProvaTavoloovo.ovo");
    if (!root)
    {
       return -1;
    }

    auto tavolo = root->findNodeByName("baseTavolo");
    if (!tavolo)
    {
       std::cout << "Tavolo non trovato" << std::endl;
       return -1;
    }
    auto bordoTavolo = root->findNodeByName("bordoTavolo");
    auto gamba1 = root->findNodeByName("gamba1");
    auto gamba2 = root->findNodeByName("gamba2");
    auto gamba3 = root->findNodeByName("gamba3");
    auto gamba4 = root->findNodeByName("gamba4");

    auto torreDiHanoi = root->findNodeByName("torreDiHanoi");
    auto baseHanoi = root->findNodeByName("baseHanoi");

    auto palo1 = root->findNodeByName("palo1");
    auto palo2 = root->findNodeByName("palo2");
    auto palo3 = root->findNodeByName("palo3");


    auto disco1 = root->findNodeByName("disco1");
    auto disco2 = root->findNodeByName("disco2");
    auto disco3 = root->findNodeByName("disco3");
    auto disco4 = root->findNodeByName("disco4");
    auto disco5 = root->findNodeByName("disco5");
    auto disco6 = root->findNodeByName("disco6");
    auto disco7 = root->findNodeByName("disco7");


    if (!palo1 || !palo2 || !palo3) {
       std::cerr << "ERRORE: Non ho trovato i pali! Controlla i nomi in 3ds Max." << std::endl;
       // Suggerimento di debug: stampa la struttura se non trova nulla
       //root->printStructure(); 
       return -1;
    }

    if (!disco1) std::cerr << "WARNING: disco1 non trovato." << std::endl;


    const auto cam = eng.createPerspectiveCamera(45, 800.f/600.f, 0.1f, 100.0f);
    cam->setMatrix(glm::translate(glm::mat4(1), glm::vec3(0,0,5)));

    const auto renderList = std::make_shared<Eng::List>();
    renderList->pass(root);

    float angle = 0.0f;

    while (eng.update()) {
        eng.clear();

        renderList->clear();

        // carichiamo matrice di proiezione
        eng.begin3D(cam);

        renderList.get()->pass(root);
        eng.showFps();
        eng.render(cam, renderList);


        eng.end3D();

        eng.swap();
    }

    return 0;
}

