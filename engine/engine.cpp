#include "engine.h"
#include <iostream>   
#include <source_location>
#include <GL/freeglut.h>

struct Eng::Base::Reserved
{
   // Flags:
   bool initFlag;
   
   Reserved() : initFlag{ false } 
   {}
};

ENG_API Eng::Base::Base() : reserved_(std::make_unique<Reserved>())
{  
#ifdef _DEBUG   
   std::cout << "[+] " << std::source_location::current().function_name() << " invoked" << std::endl;
#endif
}

ENG_API Eng::Base::~Base()
{
#ifdef _DEBUG
   std::cout << "[-] " << std::source_location::current().function_name() << " invoked" << std::endl;
#endif
}

ENG_API Eng::Base& Eng::Base::getInstance()
{
   static Base instance;
   return instance;
}

ENG_API int Eng::Base::getWindowId() const noexcept {
   return windowId_;
}


ENG_API void Eng::Base::setWindowId(const int windowId) noexcept {
   windowId_ = windowId;
}


// const rimosso a causa del setWindowID
ENG_API bool Eng::Base::init(int argc, char **argv)  {
   // Already initialized?
   if (reserved_->initFlag)
   {
      std::cout << "ERROR: engine already initialized" << std::endl;
      return false;
   }

   // Here you can initialize most of the graphics engine's dependencies and default settings...

   // Init context:
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowPosition(100, 100);


   // FreeGLUT can parse command-line params
   // se passo il valore diretto il programma crusha
   glutInit(&argc, argv);

   // Set some optional flags:
   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

   // Create the window with a specific title:
   setWindowId(glutCreateWindow("Hanoi Tower"));


   // Global OpenGL settings:
   glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);



   
   // Done:
   std::cout << "[>] " << LIB_NAME << " initialized" << std::endl;
   reserved_->initFlag = true;
   return true;
}



ENG_API bool Eng::Base::free() const {
   // Not initialized?
   if (!reserved_->initFlag)
   {
      std::cout << "ERROR: engine not initialized" << std::endl;
      return false;
   }

   // Here you can properly dispose of any allocated resource (including third-party dependencies)...

   // Done:
   std::cout << "[<] " << LIB_NAME << " deinitialized" << std::endl;
   reserved_->initFlag = false;
   return true;
}
