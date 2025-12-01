#include "engine.h"


int main(const int argc, char** argv) {

   Eng::Base &eng = Eng::Base::getInstance();


   eng.init(argc, argv, "Hanoi Tower");

    std::shared_ptr<Eng::Node> root = eng.load("nullptr");

    auto cam = eng.createPerspectiveCamera(45, 800/600.0f, 0.1f, 100.0f);
    cam->setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0,0,5)));
    glm::mat4 view = glm::inverse(cam->getMatrix());


    root->render(glm::mat4(1), view);



   while (eng.update()) {
      eng.clear();

       root->render(glm::mat4(1.0f), glm::mat4(1.0f));


      eng.swap();
   }

   return 0;
}
