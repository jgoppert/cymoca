# cymoca

[![Build Status](https://travis-ci.org/jgoppert/cymoca.svg?branch=master)](https://travis-ci.org/jgoppert/cymoca)
[![Coverage Status](https://coveralls.io/repos/github/jgoppert/cymoca/badge.svg?branch=master)](https://coveralls.io/github/jgoppert/cymoca?branch=master)

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
* C++17 used throughout, minimal copying, low memory footprint, and fast.
* Python bindings planned.
* Data format for simulations [HDF5](https://www.hdfgroup.org/)

## Road Map

* [x] Parsing with ANTLR.
* [x] Lisp listener for AST string output.
* [ ] Bouncing ball end-to-end prototype from Modelica -> Casadi. Simulation in Casadi or using sundials directly.
* [ ] Flesh out missing language element.
* [ ] NuSMV output for state machines to support model checking.
* [ ] Python bindings.

## Command Line Interface

This command line just prints the LISP generator output currently, but it will be extended to support various generators.

```bash
$ ./cymoca -h
Cymoca Modelica Compiler 0.0.9-60-g9dd7a23
usage: cymoca [options] model.mo
Options:

Generic options:
  -h [ --help ]          print help message

$ ./cymoca ./test/models/BouncingBall.mo 
(class(dict of comp( v(Real))(parameter g(Real))( x(Real))
)(list of eq(=(der((x)))(v))(=(der((v)))(-(g)))(when(eq block(<(x)(0))(list of eq(=(v)(-(v))))))))
```

## Install

### Binary

* Binaries are provided here: [releases](https://github.com/jgoppert/cymoca/releases/latest).
* The cymoca-...-Linux.zip is statically linked so it should work on all linux variants and Mac out of the box given the
following dependencies are met:
* Dependencies
   * glibc 2.2.5

### Build from Source

* You can build from source using cmake.
* Dependencies
   * Antlr4 C++ runtime:
     * Ubuntu 18.04: https://github.com/jgoppert/antlr4/releases/download/4.7.1-cpack/LIBANTLR4-4.7.1-Linux.deb
     * Mac: http://www.antlr.org/download/antlr4-cpp-runtime-4.7.1-macos.zip
     * Windows: http://www.antlr.org/download/antlr4-cpp-runtime-4.7.1-vs2015.zip
     * Other: http://www.antlr.org/download/antlr4-cpp-runtime-4.7.1-source.zip
   * gtest, optional, (automatically downloaded and built with cmake) enabled with -DWITH_TEST=ON
   * libboost-filesystem-dev
   * libboost-program-options-dev
   * casadi, optional, requires system installed headers and libraries, enabled with -DWITH_CASADI=ON
     * Ubuntu 18.04: https://github.com/jgoppert/casadi/releases/download/3.4.4-jmg/casadi-3.4.4-Linux.deb
     * Other: build using cmake: https://github.com/casadi/casadi/wiki/InstallationInstructions

* A quick example of building with cmake:
```bash
git clone https://github.com/jgoppert/cymoca
cd cymoca
mkdir -p build
cd build && cmake -DWITH_TEST=ON ..
make -j4
ctest
cpack -G DEB
sudo dpkg -i cymoca*.deb
cymoca my_model.mo
```

## Contributing

* Coding standard: [LLVM](https://llvm.org/docs/CodingStandards.html)
* If you want to get started, feel free to make a PR. The easiest place to start is with a new generator derived from Listener/ConstListener.

## License

* BSD-3-clause, open source and free for commercial use. [LICENSE](LICENSE)
