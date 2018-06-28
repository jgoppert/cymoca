# cymoca

[![Build Status](https://travis-ci.org/jgoppert/cymoca.svg?branch=master)](https://travis-ci.org/jgoppert/cymoca)

A port of the [pymoca](https://pymoca.com) Modelica compiler to C++.

* Parsing using ANTLR4 C++ Runtime.
* First target, Casadi

## RoadMap

* [x] Parsing with ANTLR
* [x] Lisp listener for AST string output.
* [ ] Bouncing ball end-to-end prototype from Modelica -> Casadi. Simulation in Casadi or using sundials directly.
* [ ] Flesh out missing language element.
* [ ] NuSMV output for state machines to support model checking.

## Install

* Binaries are provided here: [binaries](https://github.com/jgoppert/cymoca/releases). The cymoca-...-Linux.zip is statically linked to all dependencies and only requires glibc 2.2.5 to be installed. This means it should work on all linux variants and Mac out of the box.
* You can build from source using cmake.
