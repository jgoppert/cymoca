//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "cymoca_compiler/ast/ast.h"

#include <iostream>
#include <stack>

using namespace std;
using namespace cymoca::ast;

namespace cymoca::listener {

/**
 * A listener to flatten the class hierarchy.
 */
class Flattener : public Listener {
public:
  void enter(Class &ctx) override { _stack.push(&ctx); }
  void exit(Class &ctx) override {
    vector<string> keysToDelete;
    vector<pair<string, unique_ptr<Component>>> pairsToAdd;
    auto &dict = ctx.components();
    for (auto &iter : dict.memory()) {
      auto name = iter.first;
      auto &comp = *(iter.second);
      // handle predefined types, see Spec 4.8 Predefined
      // Types and Classes
      if (comp.type().name() == "Real") {
        // remove existing component
        keysToDelete.push_back(iter.first);

        // add new components
        // TODO handle default values
        // RealType value; Accessed without dot-notation
        pairsToAdd.push_back(
            {name, make_unique<Component>(name, "RealType", comp.prefix())});
        // parameter StringType quantity = "";
        pairsToAdd.push_back(
            {name + ".quantity",
             make_unique<Component>(name + ".quantity", "StringType",
                                    Prefix::PARAMETER)});
        // parameter StringType unit = "" "Unit used in
        // equations";
        pairsToAdd.push_back({name + ".unit", make_unique<Component>(
                                                  name + ".unit", "StringType",
                                                  Prefix::PARAMETER)});
        // parameter StringType displayUnit = ""
        // "Default display unit";
        pairsToAdd.push_back(
            {name + ".displayUnit",
             make_unique<Component>(name + ".displayUnit", "StringType",
                                    Prefix::PARAMETER)});
        // parameter RealType min=-Inf, max=+Inf; // Inf
        // denotes a large value
        pairsToAdd.push_back(
            {name + ".min", make_unique<Component>(name + ".min", "RealType",
                                                   Prefix::PARAMETER)});
        // parameter RealType start = 0; // Initial
        // value
        pairsToAdd.push_back(
            {name + ".max", make_unique<Component>(name + ".max", "RealType",
                                                   Prefix::PARAMETER)});
        // parameter RealType start = 0; // Initial
        // value
        pairsToAdd.push_back({name + ".start", make_unique<Component>(
                                                   name + ".start", "RealType",
                                                   Prefix::PARAMETER)});
        // parameter BooleanType fixed = true, //
        // default for parameter/constant; = false; //
        // default for other variables
        pairsToAdd.push_back(
            {name + ".fixed",
             make_unique<Component>(name + ".fixed", "BooleanType",
                                    Prefix::PARAMETER)});
        // parameter RealType nominal; // Nominal value
        pairsToAdd.push_back(
            {name + ".nominal",
             make_unique<Component>(name + ".nominal", "RealType",
                                    Prefix::PARAMETER)});
        // parameter BooleanType unbounded=false; // For
        // error control
        pairsToAdd.push_back(
            {name + ".unbounded",
             make_unique<Component>(name + ".unbounded", "BooleanType",
                                    Prefix::PARAMETER)});
        // parameter StateSelect stateSelect =
        // StateSelect.default;
        pairsToAdd.push_back(
            {name + ".stateSelect",
             make_unique<Component>(name + ".stateSelect", "StateSelect",
                                    Prefix::PARAMETER)});
        // TODO assert(value >= min and value <= max,
        // "Variable value out of limit");

      } else if (comp.type().name() == "Integer") {
        // remove existing component
        keysToDelete.push_back(iter.first);

        // TODO handle default values
        // IntegerType value; // Accessed without
        // dot-notation
        pairsToAdd.push_back(
            {name, make_unique<Component>(name, "IntegerType", comp.prefix())});
        // parameter StringType quantity = "";
        pairsToAdd.push_back(
            {name + ".quantity",
             make_unique<Component>(name + ".quantity", "StringType",
                                    Prefix::PARAMETER)});
        // parameter IntegerType min=-Inf, max=+Inf;
        pairsToAdd.push_back(
            {name + ".min", make_unique<Component>(name + ".min", "IntegerType",
                                                   Prefix::PARAMETER)});
        pairsToAdd.push_back(
            {name + ".max", make_unique<Component>(name + ".max", "IntegerType",
                                                   Prefix::PARAMETER)});
        // parameter IntegerType start = 0; // Initial
        // value
        pairsToAdd.push_back(
            {name + ".start",
             make_unique<Component>(name + ".start", "IntegerType",
                                    Prefix::PARAMETER)});
        // parameter BooleanType fixed = true, //
        // default for parameter/constant; = false; //
        // default for other variables
        pairsToAdd.push_back(
            {name + ".fixed",
             make_unique<Component>(name + ".fixed", "BooleanType",
                                    Prefix::PARAMETER)});
        // TODO equation assert(value >= min and value
        // <= max, "Variable value out of limit");

      } else if (comp.type().name() == "Boolean") {
        // remove existing component
        keysToDelete.push_back(iter.first);

        // TODO handle default values
        // BooleanType value; // Accessed without
        // dot-notation
        pairsToAdd.push_back(
            {name, make_unique<Component>(name, "BooleanType", comp.prefix())});
        // parameter StringType quantity = "";
        pairsToAdd.push_back(
            {name + ".quantity",
             make_unique<Component>(name + ".quantity", "StringType",
                                    Prefix::PARAMETER)});
        // parameter BooleanType start = false; //
        // Initial value
        pairsToAdd.push_back(
            {name + ".start",
             make_unique<Component>(name + ".start", "BooleanType",
                                    Prefix::PARAMETER)});
        // parameter BooleanType fixed = true, //
        // default for parameter/constant; = false, //
        // default for other variables
        pairsToAdd.push_back(
            {name + ".fixed",
             make_unique<Component>(name + ".fixed", "BooleanType",
                                    Prefix::PARAMETER)});

        throw logic_error("not implemented");
      } else if (comp.type().name() == "String") {
        // remove existing component
        keysToDelete.push_back(iter.first);

        // TODO handle default values
        // StringType value; // Accessed without
        // dot-notation
        pairsToAdd.push_back(
            {name, make_unique<Component>(name, "StringType", comp.prefix())});
        // parameter StringType quantity = "";
        pairsToAdd.push_back(
            {name + ".quantity",
             make_unique<Component>(name + ".quantity", "StringType",
                                    Prefix::PARAMETER)});
        // parameter StringType start = ""; // Initial
        // value
        pairsToAdd.push_back(
            {name + ".start",
             make_unique<Component>(name + ".start", "StringType",
                                    Prefix::PARAMETER)});
        // parameter BooleanType fixed = true, //
        // default for parameter/constant; = false, //
        // default for other variables
        pairsToAdd.push_back(
            {name + ".fixed",
             make_unique<Component>(name + ".fixed", "BooleanType",
                                    Prefix::PARAMETER)});

      } else if (comp.type().name() == "Enumeration") {
        // remove existing component
        keysToDelete.push_back(iter.first);

        // TODO handle default values
        // EnumType value; // Accessed without
        // dot-notation
        pairsToAdd.push_back(
            {name, make_unique<Component>(name, "EnumType", comp.prefix())});
        // parameter StringType quantity = "";
        pairsToAdd.push_back(
            {name + ".quantity",
             make_unique<Component>(name + ".quantity", "StringType",
                                    Prefix::PARAMETER)});
        // parameter EnumType min=e1, max=en;
        pairsToAdd.push_back(
            {name + ".min", make_unique<Component>(name + ".min", "EnumType",
                                                   Prefix::PARAMETER)});
        pairsToAdd.push_back(
            {name + ".max", make_unique<Component>(name + ".max", "EnumType",
                                                   Prefix::PARAMETER)});
        // parameter EnumType start = e1; // Initial
        // value
        pairsToAdd.push_back({name + ".start", make_unique<Component>(
                                                   name + ".start", "EnumType",
                                                   Prefix::PARAMETER)});
        // parameter BooleanType fixed = true, //
        // default for parameter/constant; = false; //
        // default for other variables
        pairsToAdd.push_back(
            {name + ".fixed",
             make_unique<Component>(name + ".fixed", "BooleanType",
                                    Prefix::PARAMETER)});
        // TODO add constants for enum
        // constant EnumType e1=...;
        // ...
        // constant EnumType en=...;
        throw logic_error("not implemented");

      } else {
        // handle user defined types TODO
        throw logic_error("not implemented");
      }
    }
    for (auto key : keysToDelete) {
      ctx.components().memory().erase(key);
    }
    for (auto &keyval : pairsToAdd) {
      // use set from components so that parent is set for
      // each node
      ctx.components().set(keyval.first, move(keyval.second));
    }
    _stack.pop();
  }

protected:
  stack<const Class *> _stack{};
};

} // namespace cymoca::listener

// vim: set et fenc=utf-8 ff=unix sts=0 sw=2 ts=2 :
