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

void Object::setName(const std::string& name) noexcept {
   name_ = name;
}

std::string Object::getName() const noexcept {
   return name_;
}

unsigned long Object::getId() const noexcept {
   return id_;
}

void Object::setId(const unsigned long id) noexcept {
   id_ = id;
}

void Object::render(const glm::mat4 &C, glm::mat4 M) {}
