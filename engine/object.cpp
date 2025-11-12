#include "object.h"
#include <iostream>
#include <source_location>

using namespace Eng;

ENG_API Object::Object() : id{ 0 }, name{ "Unnamed" } {
#ifdef _DEBUG
   std::cout << "[+] " << std::source_location::current().function_name() << " invoked\n";
#endif
}

ENG_API Object::~Object() {
#ifdef _DEBUG
   std::cout << "[-] " << std::source_location::current().function_name() << " invoked\n";
#endif
}

ENG_API Object::Object(const Object& other) : id{ other.id }, name{ other.name } {}

ENG_API Object::Object(Object&& other) noexcept : id{ other.id }, name{ std::move(other.name) } {}

ENG_API Object& Object::operator=(const Object& other) {
   if (this != &other) {
      id = other.id;
      name = other.name;
   }
   return *this;
}

ENG_API Object& Object::operator=(Object&& other) noexcept {
   if (this != &other) {
      id = other.id;
      name = std::move(other.name);
   }
   return *this;
}

ENG_API void Object::setName(const std::string& name_) { name = name_; }

ENG_API std::string Object::getName() const { return name; }

ENG_API unsigned long Object::getId() const { return id; }