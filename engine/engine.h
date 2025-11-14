#pragma once
#include <memory> 

#ifdef _DEBUG
   #define LIB_NAME      "My Graphics Engine v0.1a (debug)"   ///< Library credits
#else
   #define LIB_NAME      "My Graphics Engine v0.1a"   ///< Library credits
#endif
   #define LIB_VERSION   10                           ///< Library version (divide by 10)

   // Export API:
#ifdef _WINDOWS
   // Specifies i/o linkage (VC++ spec):
   #ifdef ENGINE_EXPORTS
      #define ENG_API __declspec(dllexport)
   #else
      #define ENG_API __declspec(dllimport)
   #endif      
// CONTROLLARE!!!
#pragma warning(disable : 4251)
#else
#define ENG_API __attribute__((visibility("default")))
#endif



namespace Eng {

   // You can subinclude here other headers of your engine...

   class ENG_API Base final
   {
      public:
         Base(Base const &) = delete;
         ~Base();

         // Operators:
         void operator=(Base const &) = delete;

         // Singleton:
         static Base &getInstance();

         // Init/free:
         bool init();
         bool free();   

      private:
         // Reserved:
         struct Reserved;
         std::unique_ptr<Reserved> reserved_;

         Base();
   };

};

