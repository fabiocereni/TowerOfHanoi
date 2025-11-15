#include "list.h"

using namespace Eng;

ENG_API std::list<Instance> List::getRenderList() const noexcept {
   return instances_;
}

ENG_API void List::setRenderList(const std::list<Instance> &renderList) noexcept {
   instances_ = renderList;
}

ENG_API void List::addOnBottomToRenderList(const Instance &instance) noexcept {
   instances_.push_back(instance);
}

ENG_API void List::addOnTopToRenderList(const Instance &instance) noexcept {
   instances_.push_front(instance);
}

ENG_API bool List::removeFromRenderList(const Instance &instance) noexcept {
   const size_t oldSize = instances_.size();
   instances_.remove(instance);
   return instances_.size() != oldSize;
}
