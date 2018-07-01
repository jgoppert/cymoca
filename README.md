# cymoca

[![Build Status](https://travis-ci.org/jgoppert/cymoca.svg?branch=master)](https://travis-ci.org/jgoppert/cymoca)

A Modelica compiler written in modern C++ with Python bindings.

## Features

* Parsing using [ANTLR4 C++ Runtime](https://github.com/antlr/antlr4/blob/master/doc/cpp-target.md).
	* <img src="doc/ast.png" alt="drawing" width="500px"/>
* Listener based generators decoupled from the AST, easy to compile Modelica to new targets.
  * Lisp string printing [LispListener](src/cymoca_compiler/listener/LispPrinter.h).
  * Generator for [Casadi](https://github.com/casadi/casadi/wiki) in development, [Demo](test/test_casadi.cpp).
* Listener based compiler
  * [Compiler](src/cymoca_compiler/Compiler.cpp)
  * [Tree Rewriting](src/cymoca_compiler/listener/WhenExpander.h)
* C++14 used throughout, minimal copying, low memory footprint, and fast.
* Python bindings planned.
* Data format for simulations [HDF5](https://www.hdfgroup.org/)

## Road Map

* [x] Parsing with ANTLR.
* [x] Lisp listener for AST string output.
* [ ] Bouncing ball end-to-end prototype from Modelica -> Casadi. Simulation in Casadi or using sundials directly.
* [ ] Flesh out missing language element.
* [ ] NuSMV output for state machines to support model checking.
* [ ] Python bindings.

# Command Line

This command line just prints the LISP generator output currently, but it will be extended to support various generators.

"""bash
$ ./cymoca -h
Cymoca Modelica Compiler 0.0.9-60-g9dd7a23
usage: cymoca [options] model.mo
Options:

Generic options:
  -h [ --help ]          print help message

$ ./cymoca ./test/models/BouncingBall.mo 
(class(dict of comp( v(Real))(parameter g(Real))( x(Real))
"""

## Install

* Binaries are provided here: [releases](https://github.com/jgoppert/cymoca/releases/latest). The cymoca-...-Linux.zip is statically linked to all dependencies and only requires glibc 2.2.5 to be installed. This means it should work on all linux variants and Mac out of the box.
* You can build from source using cmake.

## Contributing

* Coding standard: [LLVM](https://llvm.org/docs/CodingStandards.html)
* If you want to get started, feel free to make a PR. The easiest place to start is with a new generator derived from Listener/ConstListener.

## License

* BSD-3-clause, open source and free for commercial use. [LICENSE](LICENSE)
