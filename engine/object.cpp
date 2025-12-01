#include "object.h"
#include <iostream>
#include <source_location>


using namespace Eng;

Object::Object() noexcept
               : id_{ 0 }, name_{ "Unnamed" }  {
#ifdef _DEBUG
   std::cout << "[+] " << std::source_location::current().function_name() << " invoked\n";
#endif
}
