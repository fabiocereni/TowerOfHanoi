#include "engine.h"
#include <iostream>


int main(int argc, char *argv[]) {

   // Init engine:
   Eng::Base &eng = Eng::Base::getInstance();
   eng.init();

   // Release engine:
   eng.free();

   // Done:
   std::cout << "\n[application terminated]" << std::endl;
   return 0;
}
