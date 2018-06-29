//
// Created by jgoppert on 6/28/18.
//

#include "Binary.h"

namespace cymoca {
namespace ast {

unordered_map<BinaryOp, string, EnumClassHash> binaryOpStr = {
    {BinaryOp::ADD, "+"},
    {BinaryOp::SUB, "-"},
    {BinaryOp::MUL, "*"},
    {BinaryOp::DIV, "/"},
    {BinaryOp::POW, "^"},
    {BinaryOp::EMUL, ".*"},
    {BinaryOp::EDIV, "./"},
    {BinaryOp::EPOW, ".^"}
};

unordered_map<UnaryOp, string, EnumClassHash> unaryOpStr = {
    {UnaryOp::NEG, "-"}
};

unordered_map<RelationOp, string, EnumClassHash> relationOpStr = {
    {RelationOp::LT, "<"},
    {RelationOp::LE, "<="},
    {RelationOp::GT, ">"},
    {RelationOp::GE, ">="},
    {RelationOp::EQ, "="},
    {RelationOp::NEQ, "<>"}
};

unordered_map<BinaryLogicOp, string, EnumClassHash> binaryLogicOpStr = {
    {BinaryLogicOp::AND, "and"},
    {BinaryLogicOp::OR, "or"},
};

} // ast
} // cymoca
