#include "engine.h"
#include <iostream>   
#include <source_location>

struct Eng::Base::Reserved
{
   // Flags:
   bool initFlag;
   
   Reserved() : initFlag{ false } 
   {}
};

ENG_API Eng::Base::Base() : reserved_(std::make_unique<Eng::Base::Reserved>())
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

ENG_API bool Eng::Base::init() const {
   // Already initialized?
   if (reserved_->initFlag)
   {
      std::cout << "ERROR: engine already initialized" << std::endl;
      return false;
   }

   // Here you can initialize most of the graphics engine's dependencies and default settings...
   
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
