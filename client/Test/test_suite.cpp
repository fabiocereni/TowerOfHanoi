#include "test_suite.h"
#include <iostream>
#include <memory>
#include <engine.h>

using namespace Eng;

void TestSuite::runAllTests() {
    testNodeHierarchy();
    testNodeTransformations();
    testListManagement();
    testCameraViewMatrix();
}

void TestSuite::assertCondition(const std::string& testName, const bool condition) {
    std::cout << "Testing: " << testName << "... ";
    assert(condition && "Test Fallito!"); // Il programma si chiuderà qui se condition è false
    std::cout << "Passed\n";
}

void TestSuite::testNodeHierarchy() {
    std::cout << "--- Test Gerarchia Nodi ---\n";

    const auto parent = std::make_shared<Node>();
    const auto child = std::make_shared<Node>();

    parent->addChildren(child);
    child->setParent(parent);

    // Verifica che il padre abbia 1 figlio
    assertCondition("Parent has 1 child", parent->getChildren().size() == 1);

    // Verifica che il figlio punti al padre corretto
    assertCondition("Child has correct parent", child->getParent() == parent);

    // Verifica rimozione
    auto removed = parent->removeChildren(child->getName());
}

void TestSuite::testNodeTransformations() {
    std::cout << "--- Test Trasformazioni Matrici ---\n";

    const auto root = std::make_shared<Node>();
    const auto child = std::make_shared<Node>();

    // Root spostato di 10 su X
    constexpr glm::mat4 rootTrans = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 0.0f));
    root->setMatrix(rootTrans);

    // Child spostato di 5 su X (relativo al padre)
    constexpr glm::mat4 childTrans = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f));
    child->setMatrix(childTrans);
    child->setParent(root);

    // La posizione world del child dovrebbe essere 15 su X (10 + 5)
    glm::mat4 worldMat = child->getWorldMatrix();
    const auto worldPos = glm::vec3(worldMat[3]); // La quarta colonna contiene la posizione

    // Usiamo una piccola epsilon per i float
    const bool isCorrectX = std::abs(worldPos.x - 15.0f) < 0.001f;
    assertCondition("Child World Position X is 15.0", isCorrectX);
}

void TestSuite::testListManagement() {
    std::cout << "--- Test Gestione RenderList ---\n";

    List renderList;
    const auto node = std::make_shared<Node>();

    // Simula il passaggio nel grafo
    renderList.pass(node, glm::mat4(1.0f));

    assertCondition("List is not empty after pass", !renderList.getRenderList().empty());
    assertCondition("List size is 1", renderList.getRenderList().size() == 1);

    renderList.clear();
    assertCondition("List is empty after clear", renderList.getRenderList().empty());
}

void TestSuite::testCameraViewMatrix() {
    std::cout << "--- Test Camera View Matrix ---\n";

    // Crea una camera prospettica
    const auto cam = std::make_shared<Perspective_Camera>(45.0f, 1.33f, 0.1f, 100.0f);

    // Sposta la camera a (0, 0, 10)
    constexpr glm::mat4 camPos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 10.0f));
    cam->setMatrix(camPos);

    // La View Matrix è l'inversa della World Matrix della camera.
    // L'inversa di una traslazione (0,0,10) è una traslazione (0,0,-10).
    glm::mat4 viewMat = cam->getViewMatrix();
    const auto viewTranslation = glm::vec3(viewMat[3]);

    const bool isCorrectZ = std::abs(viewTranslation.z - (-10.0f)) < 0.001f;
    assertCondition("View Matrix translates scene by -10 Z", isCorrectZ);
}