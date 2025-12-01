#include "engine.h"


int main(int argc, char** argv) {

    Eng::Base& eng = Eng::Base::getInstance();
    eng.init(argc, argv, "Hanoi Tower");

    auto root = eng.load("nullptr");

    auto cam = eng.createPerspectiveCamera(45, 800.f/600.f, 0.1f, 100.0f);
    cam->setMatrix(glm::translate(glm::mat4(1), glm::vec3(0,0,5)));

    float angle = 0.0f;

    while (eng.update()) {
        eng.clear();

        // PROJECTION
        eng.begin3D(cam);

        // ROTAZIONE
        angle += 1.0f;
        root->setMatrix(glm::rotate(glm::mat4(1), glm::radians(angle), glm::vec3(0,1,0)));
        root->setMatrix(root->getMatrix() * glm::rotate(glm::mat4(1), glm::radians(angle), glm::vec3(1,0,0)));

        // VIEW
        glm::mat4 view = glm::inverse(cam->getWorldMatrix());

        // RENDER DELLA SCENA
        root->render(glm::mat4(1), view);

        // SWAP
        eng.swap();
    }

    return 0;
}

