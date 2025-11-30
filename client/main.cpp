#include "engine.h"


int main(const int argc, char** argv) {

   Eng::Base &eng = Eng::Base::getInstance();


   eng.init(argc, argv, "Hanoi Tower");

    std::shared_ptr<Eng::Node> root = eng.load("nullptr");

    eng.changeBackgroundColor(1,1,1);

   while (eng.update()) {
      eng.clear();

       root->render(glm::mat4(1.0f));


      eng.swap();
   }

   return 0;
}
