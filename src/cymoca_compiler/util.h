//
// Created by jgoppert on 6/28/18.
//

#pragma once

using namespace std;

template<typename Derived, typename Base>
unique_ptr<Derived>
static_unique_ptr_cast(unique_ptr<Base> &&p) {
  auto d = static_cast<Derived *>(p.release());
  assert(d);
  return unique_ptr<Derived>(d);
}