//
// Created by jgoppert on 6/28/18.
//

#include "Binary.h"

namespace cymoca {
namespace ast {

map<BinaryOp, string> binaryOpStr = {
    {BinaryOp::ADD, "+"},
    {BinaryOp::SUB, "-"},
    {BinaryOp::MUL, "*"},
    {BinaryOp::DIV, "/"},
    {BinaryOp::POW, "^"},
    {BinaryOp::EMUL, ".*"},
    {BinaryOp::EDIV, "./"},
    {BinaryOp::EPOW, ".^"}
};

map<UnaryOp, string> unaryOpStr = {
    {UnaryOp::NEG, "-"}
};

map<RelationOp, string> relationOpStr = {
    {RelationOp::LT, "<"},
    {RelationOp::LE, "<="},
    {RelationOp::GT, ">"},
    {RelationOp::GE, ">="},
    {RelationOp::EQ, "="},
    {RelationOp::NEQ, "<>"}
};

map<BinaryLogicOp, string> binaryLogicOpStr = {
    {BinaryLogicOp::AND, "and"},
    {BinaryLogicOp::OR, "or"},
};

} // ast
} // cymoca
