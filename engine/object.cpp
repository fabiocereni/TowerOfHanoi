#include "object.h"
#include <iostream>
#include <source_location>


using namespace Eng;

unsigned long Object::globalId_ = 0;

Object::Object() noexcept
               : id_{ globalId_++ }, name_{ "Unnamed" }  {
#ifdef _DEBUG
   std::cout << "[+] " << std::source_location::current().function_name() << " invoked\n";
#endif
}
