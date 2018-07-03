//
// Created by jgoppert on 6/28/18.
//

#include "Unary.h"

namespace cymoca::ast {

unordered_map<UnaryOp, string, EnumClassHash> unaryOpStr = {
    {UnaryOp::NEG, "-"}};

unordered_map<UnaryLogicOp, string, EnumClassHash> unaryLogicOpStr = {
    {UnaryLogicOp::NOT, "not"}};

} // namespace cymoca::ast

// vim: set et fenc=utf-8 ff=unix sts=0 sw=2 ts=2 :
