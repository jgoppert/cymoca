//
// Created by jgoppert on 6/28/18.
//

#include "Unary.h"

namespace cymoca {
namespace ast {

unordered_map<UnaryOp, string, EnumClassHash> unaryOpStr = {
    {UnaryOp::NEG, "-"}
};

unordered_map<UnaryLogicOp, string, EnumClassHash> unaryLogicOpStr = {
    {UnaryLogicOp::NOT, "not"}
};

} // ast
} // cymoca
