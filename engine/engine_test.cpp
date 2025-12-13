#include <cassert>
#include <iostream>
#include <memory>
#include "light_number_exception.h"
#include "node.h"
#include <glm/glm.hpp>

#include "list.h"
#include "perspective_camera.h"
#include "spotlight.h"

using namespace Eng;

double delta_ = 0.00000000001;


void assertCondition(const std::string& testName, const bool condition) {
    std::cout << "Testing: " << testName << "... ";
    if (!condition) {
        std::cout << "FAILED!\n";
        assert(false && "Test Failed");
    }
    std::cout << "Passed\n";
}

void testNodeHierarchy() {
    std::cout << "\n--- Test Gerarchia Nodi ---\n";

    const auto parent = std::make_shared<Node>();
    const auto child1 = std::make_shared<Node>();
    const auto child2 = std::make_shared<Node>();

    parent->setName("ParentNode");
    child1->setName("Child1");
    child2->setName("Child2");

    parent->addChildren(child1);
    child1->setParent(parent);

    parent->addChildren(child2);
    child2->setParent(parent);

    // Verifica numero figli
    assertCondition("Parent has 2 children", parent->getChildren().size() == 2);

    // Verifica puntatore parent
    assertCondition("Child1 has correct parent", child1->getParent() == parent);

    // Verifica ricerca per nome
    auto foundNode = parent->returnChild("Child1");
    assertCondition("returnChild finds 'Child1'", foundNode != nullptr && foundNode->getName() == "Child1");

    auto notFoundNode = parent->returnChild("Ghost");
    assertCondition("returnChild returns null for non-existent node", notFoundNode == nullptr);

    // Verifica rimozione
    auto removed = parent->removeChild("Child1");
    assertCondition("Child1 removed successfully", removed != nullptr && removed->getName() == "Child1");
    assertCondition("Parent has 1 child after removal", parent->getChildren().size() == 1);
}

void testNodeTransformations() {
    std::cout << "\n--- Test Trasformazioni Matrici ---\n";

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
    const bool isCorrectX = std::abs(worldPos.x - 15.0f) < delta_;
    const bool isCorrectY = std::abs(worldPos.y - 0.0f) < delta_;

    assertCondition("Child World Position X is 15.0", isCorrectX);
    assertCondition("Child World Position Y is 0.0", isCorrectY);
}

void testListManagement() {
    std::cout << "\n--- Test Gestione RenderList ---\n";

    List renderList;
    const auto node = std::make_shared<Node>();
    node->setName("TestNode");

    // Creiamo una istanza manuale per testare add/remove
    Instance inst(node, glm::mat4(1.0f));

    // Test aggiunta manuale
    renderList.addOnBottomToRenderList(inst);
    assertCondition("List size is 1 after add", renderList.getRenderList().size() == 1);

    // Test rimozione (sfrutta l'operator== implementato in instance.h)
    bool removed = renderList.removeFromRenderList(inst);
    assertCondition("Instance removed successfully", removed);
    assertCondition("List is empty after remove", renderList.getRenderList().empty());

    // Test metodo pass() (Attraversamento grafo)
    renderList.pass(node, glm::mat4(1.0f));
    assertCondition("List populated by pass()", renderList.getRenderList().size() == 1);

    renderList.clear();
    assertCondition("List is empty after clear", renderList.getRenderList().empty());
}

void testCameraLogic() {
    std::cout << "\n--- Test Logica Camera ---\n";

    // Crea una camera prospettica
    float initialFov = 45.0f;
    auto cam = std::make_shared<Perspective_Camera>(initialFov, 1.33f, 0.1f, 100.0f);

    // 1. Test View Matrix (Inversa della World)
    // Sposta la camera a (0, 0, 10)
    constexpr glm::mat4 camPos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 10.0f));
    cam->setMatrix(camPos);

    glm::mat4 viewMat = cam->getViewMatrix();
    const auto viewTranslation = glm::vec3(viewMat[3]);

    // L'inversa di una traslazione +10 su Z è -10 su Z
    const bool isCorrectZ = std::abs(viewTranslation.z - (-10.0f)) < delta_;
    assertCondition("View Matrix translates scene by -10 Z", isCorrectZ);

    // Aggiornamento Parametri Proiezione
    cam->setFov(90.0f);
    assertCondition("FOV updated correctly via setter", std::abs(cam->getFov() - 90.0f) < delta_);

    // Controlliamo che la matrice di proiezione non sia identità (è stata calcolata)
    assertCondition("Projection Matrix is not identity", cam->getProjectionMatrix() != glm::mat4(1.0f));
}

void testLightFactoryLimits() {
    std::cout << "\n--- Test Limiti Factory Luci ---\n";


    bool exceptionThrown = false;



    const int initialCount = Spotlight::getLightNumber();

    const int lightsToCreate = 8 - initialCount; // Riempiamo gli slot rimanenti

    try {
        std::vector<std::shared_ptr<Spotlight>> lights;

        for (int i = 0; i < lightsToCreate; ++i) {
            lights.push_back(Spotlight::createSpotLight());
        }

        assertCondition("Light slots filled (8/8)", Spotlight::getLightNumber() == 8);

        auto extraLight = Spotlight::createSpotLight(); // Dovrebbe lanciare eccezione

    } catch (const LightNumberExceededException& e) {
        exceptionThrown = true;
        std::cout << "   (Caught expected exception: " << e.what() << ")\n";
    } catch (...) {
        assertCondition("Caught wrong exception type", false);
    }

    assertCondition("Exception thrown when exceeding 8 lights", exceptionThrown);

}



void testObjectIdIncrement() {
    std::cout << "\n--- Test Incremento ID Oggetti ---\n";

    class MockObject : public Object {
    public:
        void render(const glm::mat4& m) override {}
    };

    const MockObject obj1;
    const MockObject obj2;
    const MockObject obj3;

    std::cout << "Obj1 ID: " << obj1.getId() << "\n";
    std::cout << "Obj2 ID: " << obj2.getId() << "\n";
    std::cout << "Obj3 ID: " << obj3.getId() << "\n";

    // Gli ID devono essere diversi
    assertCondition("Objects have unique IDs", obj1.getId() != obj2.getId());

    //  L'ordine deve essere incrementale
    assertCondition("IDs are incremental", obj2.getId() == obj1.getId() + 1);
    assertCondition("IDs are incremental", obj3.getId() == obj2.getId() + 1);
}



int main() {

    testNodeHierarchy();
    testNodeTransformations();
    testListManagement();
    testCameraLogic();
    testLightFactoryLimits();
    testObjectIdIncrement();

}