#include "object.h"
#include <iostream>
#include <source_location>

using namespace Eng;

ENG_API Object::Object() noexcept
               : id_{ 0 }, name_{ "Unnamed" }  {
#ifdef _DEBUG
   std::cout << "[+] " << std::source_location::current().function_name() << " invoked\n";
#endif
}

ENG_API void Object::setName(const std::string& name) noexcept {
   name_ = name;
}

ENG_API std::string Object::getName() const noexcept {
   return name_;
}

ENG_API unsigned long Object::getId() const noexcept {
   return id_;
}

ENG_API void Object::setId(const unsigned long id) noexcept {
   id_ = id;
}

ENG_API void Object::render() {}
