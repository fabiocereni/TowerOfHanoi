#include "engine.h"
#include <iostream>

#include "camera.h"
#include "node.h"

int main(int argc, char *argv[]) {

   // Init engine:
   Eng::Base &eng = Eng::Base::getInstance();

   eng.init(argc, argv);

   std::shared_ptr<Eng::Node> root;
   auto camera = dynamic_cast<std::shared_ptr<Eng::Camera>>(root->find("Camera"));


   while(isRunning)
   {
      processEvents();
      clearBuffers();
      render3D(root, camera);
      swapBuffers();
   }

   // Release engine:
   eng.free();

   // Done:
   std::cout << "\n[application terminated]" << std::endl;
   return 0;
}
