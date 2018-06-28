//
// Created by jgoppert on 6/28/18.
//

#include "Node.h"

namespace cymoca {
namespace ast {

Node::Node(const type_info &type) : _type(type), _parent(nullptr) {
}

} // ast
} // cymoca