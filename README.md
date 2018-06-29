# cymoca

[![Build Status](https://travis-ci.org/jgoppert/cymoca.svg?branch=master)](https://travis-ci.org/jgoppert/cymoca)

A Modelica compiler written in modern C++ with Python bindings.

## Features

* Parsing using [ANTLR4 C++ Runtime](https://github.com/antlr/antlr4/blob/master/doc/cpp-target.md).
* Listener based generators, easily compile Modelica to any desired output
  * Basic Example: [LispListener](src/cymoca_compiler/listener/LispPrinter.h).
* Listener based compiler, easy to read and extend
  * [Tree Rewriting](src/cymoca_compiler/listener/WhenExpander.h)
  * [Compiler](src/cymoca_compiler/Compiler.cpp)
* C++14 used throughout, minimal copying, low memory footprint, and fast.
* Generator for [Casadi](https://github.com/casadi/casadi/wiki) in development.
* Python bindings planned.

## Road Map

* [x] Parsing with ANTLR.
* [x] Lisp listener for AST string output.
* [ ] Bouncing ball end-to-end prototype from Modelica -> Casadi. Simulation in Casadi or using sundials directly.
* [ ] Flesh out missing language element.
* [ ] NuSMV output for state machines to support model checking.
* [ ] Python bindings.

## Install

* Binaries are provided here: [releases](https://github.com/jgoppert/cymoca/releases/latest). The cymoca-...-Linux.zip is statically linked to all dependencies and only requires glibc 2.2.5 to be installed. This means it should work on all linux variants and Mac out of the box.
* You can build from source using cmake.

## License

* BSD-3-clause, open source and free for commercial use. [LICENSE](LICENSE)
