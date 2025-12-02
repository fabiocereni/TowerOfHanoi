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
    eng.init(argc, argv, "Hanoi Tower");

    const auto root = eng.load("nullptr");

    const auto cam = eng.createPerspectiveCamera(45, 800.f/600.f, 0.1f, 100.0f);
    cam->setMatrix(glm::translate(glm::mat4(1), glm::vec3(0,0,5)));

    const auto renderList = std::make_shared<Eng::List>();

    float angle = 0.0f;

    while (eng.update()) {
        eng.clear();

        renderList->clear();

        // carichiamo matrice di proiezione
        eng.begin3D(cam);

        // rotazione
        angle += 1.0f;
        root->setMatrix(glm::rotate(glm::mat4(1), glm::radians(angle), glm::vec3(0,1,0)));
        root->setMatrix(root->getMatrix() * glm::rotate(glm::mat4(1), glm::radians(angle), glm::vec3(1,0,0)));

        renderList.get()->pass(root);
        eng.showFps();
        eng.render(cam, renderList);


        // SWAP
        eng.swap();
    }

    return 0;
}

