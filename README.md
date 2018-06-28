# cymoca

[![Build Status](https://travis-ci.org/jgoppert/cymoca.svg?branch=master)](https://travis-ci.org/jgoppert/cymoca)

A port of the [pymoca](https://pymoca.com) Modelica compiler to C++.

## Features

* Parsing using [ANTLR4 C++ Runtime](https://github.com/antlr/antlr4/blob/master/doc/cpp-target.md)
* Listener based compiler, easy to read and extend. [example: LispListener](https://github.com/jgoppert/cymoca/blob/master/src/cymoca_compiler/listener/LispPrinter.h)
* Unique_ptr C++14 used throughout, minimal copying, low memory footprint, and fast.

## RoadMap

* [x] Parsing with ANTLR
* [x] Lisp listener for AST string output.
* [ ] Bouncing ball end-to-end prototype from Modelica -> Casadi. Simulation in Casadi or using sundials directly.
* [ ] Flesh out missing language element.
* [ ] NuSMV output for state machines to support model checking.

## Install

* Binaries are provided here: [binaries](https://github.com/jgoppert/cymoca/releases). The cymoca-...-Linux.zip is statically linked to all dependencies and only requires glibc 2.2.5 to be installed. This means it should work on all linux variants and Mac out of the box.
* You can build from source using cmake.
