//
// Created by jgoppert on 3/18/18.
//

#ifndef CYMOCA_UTILS_H
#define CYMOCA_UTILS_H

#include <string>
#include <typeinfo>

std::string demangle(const char* name);

template <class T>
std::string type(const T& t) {

  return demangle(typeid(t).name());
}

#endif //CYMOCA_UTILS_H
