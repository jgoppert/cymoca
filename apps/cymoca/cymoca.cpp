#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <cymoca_compiler/Compiler.h>
#include <cymoca_compiler/version.h>

using namespace boost::filesystem;
namespace po = boost::program_options;
using namespace std;

namespace {
const size_t ERROR_IN_COMMAND_LINE = 1;
const size_t SUCCESS = 0;
const size_t ERROR_UNHANDLED_EXCEPTION = 2;

} // namespace 


void usage(
    const string &appName,
    const po::options_description &desc,
    const po::positional_options_description &pos_desc) {
  std::cout << "Cymoca Modelica Compiler " << VERSION << std::endl;
  std::cout << "usage: " << appName << " [options] model.mo" << std::endl;
  std::cout << desc << std::endl;
  //std::cout << pos_desc << std::endl;
}

//------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
  try {
    std::string appName = boost::filesystem::basename(argv[0]);
    std::string model;

    /** Define and parse the program options 
     */
    namespace po = boost::program_options;

    po::options_description generic("Generic options");
    generic.add_options()
        ("help,h", "print help message");

    po::options_description hidden;
    hidden.add_options()
        ("model", po::value<string>(&model)->required(), "the modelica file to compile");

    po::options_description cmdline_options;
    cmdline_options.add(generic).add(hidden);

    po::options_description visible("Options");
    visible.add(generic);

    po::positional_options_description positionalOptions;
    positionalOptions.add("model", -1);

    po::variables_map vm;

    try {
      po::store(po::command_line_parser(argc, argv).
                    options(cmdline_options).positional(positionalOptions).run(),
                vm); // throws on error

      /** --help option 
       */
      if (vm.count("help")) {
        usage(appName, visible, positionalOptions);
        return SUCCESS;
      }

      po::notify(vm); // throws on error, so do after help in case 
      // there are any problems
    }
    catch (boost::program_options::required_option &e) {
      //rad::OptionPrinter::formatRequiredOptionError(e); 
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
      usage(appName, visible, positionalOptions);
      return ERROR_IN_COMMAND_LINE;
    }
    catch (boost::program_options::error &e) {
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
      usage(appName, visible, positionalOptions);
      return ERROR_IN_COMMAND_LINE;
    }

    // horizontal bar for output
    std::string bar = "";
    for (int i=0; i<78; i++) {
      bar += "=";
    }

    // compile
    path model_path(vm["model"].as<string>());
    assert(exists(model_path));
    std::ifstream fileStream(model_path.string());
    cymoca::Compiler c(fileStream);

    std::cout << bar << "\nLexer\n" << bar << std::endl;
    for (auto token : c.getTokenStream().getTokens()) {
      std::cout << token->toString() << std::endl;
    }

    std::cout << bar << "\nParse Tree\n" << bar << std::endl;
    std::cout << cymoca::toPrettyStringTree(
        c.getRoot(),
        c.getParser().getRuleNames(),
        c.getAst()) << std::endl;

    std::cout << bar << "\nModelicaXML\n" << bar<< std::endl;
    c.printXML(std::cout);
  }
  catch (std::exception &e) {
    std::cerr << "Unhandled Exception reached the top of main: "
              << e.what() << ", application will now exit" << std::endl;
    return ERROR_UNHANDLED_EXCEPTION;

  }

  return SUCCESS;

} // main 
/* vim: set et fenc=utf-8 ff=unix sts=0 sw=2 ts=2 : */
