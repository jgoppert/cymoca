#include <iostream>
#include <experimental/filesystem>
#include "cymoca/compiler.h"
#include "cymoca/version.h"
#include "cymoca/ast/listener/lisp.h"

namespace fs = std::experimental::filesystem;

void usage() {
  std::cout << "Cymoca Modelica Compiler " << VERSION << std::endl;
  std::cout << "usage: cymoca model.mo" << std::endl;
}

//------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
  if (argc < 2) {
    usage();
    return 0;
  }
  try {
    std::string appName = argv[0];
    std::string model = argv[1]; 

    // horizontal bar for output
    std::string bar = "";
    for (int i = 0; i < 78; i++) {
      bar += "=";
    }

    // compile
    fs::path model_path(model);
    if (!exists(model_path)) {
      std::cerr << "path does not exist: " << model << std::endl;
      return 0;
    }
    std::ifstream fileStream(model_path.string());
    cymoca::Compiler c(fileStream);

    //std::cout << bar << "\nLexer\n" << bar << std::endl;
    //for (auto token : c.getTokenStream().getTokens()) {
    //  std::cout << token->toString() << std::endl;
    //}

    //std::cout << bar << "\nParse Tree\n" << bar << std::endl;
    //std::cout << c.toPrettyStringTree() << std::endl;

    //std::cout << bar << "\nLisp\n" << bar<< std::endl;
    cymoca::ast::listener::Lisp printer;
    cymoca::ast::Walker walker;
    if (!c.root()) {
      throw std::runtime_error("failed to compile model");
    }
    walker.walk(*c.root(), printer);
    std::cout << printer.get() << std::endl;

  }
  catch (std::exception &e) {
    std::cerr << "Unhandled Exception reached the top of main: "
         << e.what() << ", application will now exit" << std::endl;
    return -1;

  }

  return 0;

} // main 
