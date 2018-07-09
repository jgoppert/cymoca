#ifndef CYMOCA_UTIL_H_
#define CYMOCA_UTIL_H_

#include <memory>

template <typename Derived, typename Base>
std::unique_ptr<Derived> static_unique_ptr_cast(std::unique_ptr<Base> &&p) {
  auto d = static_cast<Derived *>(p.release());
  assert(d);
  return std::unique_ptr<Derived>(d);
}

struct EnumClassHash {
  template <typename T>
  size_t operator()(T t) const {
    return static_cast<size_t>(t);
  }
};

#endif
