#include "engine.h"
#include <iostream>

int main(const int argc, char** argv) {

    Eng::Base& eng = Eng::Base::getInstance();
    eng.init(argc, argv, "Hanoi Tower Debug");

    // 1. Carica il cubo hardcodato (che è dentro 'root')
    std::shared_ptr<Eng::Node> root = eng.load("nullptr");

    // 2. Crea una Camera Prospettica
    // FOV: 60 gradi, Aspect Ratio: larghezza/altezza, Near: 0.1, Far: 100
    float aspect = (float)eng.getWidth() / (float)eng.getHeight();
    std::shared_ptr<Eng::Camera> camera = Eng::Base::createPerspectiveCamera(60.0f, aspect, 0.1f, 100.0f);

    // 3. Sposta la Camera indietro per vedere il cubo
    // Nota: La camera in OpenGL guarda verso -Z. 
    // Spostiamola a +5 su Z e un po' in alto su Y.
    glm::mat4 camPos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 5.0f));
    // La camera deve anche "guardare" l'oggetto. Per semplicità ora la ruotiamo un po' verso il basso
    camPos = glm::rotate(camPos, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    camera->setMatrix(camPos);

    eng.changeBackgroundColor(0.1f, 0.1f, 0.1f); // Sfondo grigio scuro per contrasto

    while (eng.update()) {
        eng.clear();

        // Imposta la matrice di PROIEZIONE (gestita da begin3D o manualmente)
        Eng::Base::begin3D(camera);

        // Calcola la View Matrix (inversa della posizione della camera)
        glm::mat4 viewMatrix = camera->getViewMatrix();

        // Renderizza la scena passando la View Matrix come "parent" iniziale
        root->render(viewMatrix);

        // Eng::Base::end3D(); // Se implementata
        eng.swap();
    }

    return 0;
}
