#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <cymoca_compiler/Compiler.h>
#include <cymoca_compiler/version.h>
#include <cymoca_compiler/listener/LispPrinter.h>

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
  cout << "Cymoca Modelica Compiler " << VERSION << endl;
  cout << "usage: " << appName << " [options] model.mo" << endl;
  cout << desc << endl;
  (void)pos_desc; // ignore unused
}

//------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
  try {
    string appName = boost::filesystem::basename(argv[0]);
    string model;

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
      cerr << "ERROR: " << e.what() << endl << endl;
      usage(appName, visible, positionalOptions);
      return ERROR_IN_COMMAND_LINE;
    }
    catch (boost::program_options::error &e) {
      cerr << "ERROR: " << e.what() << endl << endl;
      usage(appName, visible, positionalOptions);
      return ERROR_IN_COMMAND_LINE;
    }

    // horizontal bar for output
    string bar = "";
    for (int i=0; i<78; i++) {
      bar += "=";
    }

    // compile
    path model_path(vm["model"].as<string>());
    assert(exists(model_path));
    ifstream fileStream(model_path.string());
    cymoca::Compiler c(fileStream);

    //cout << bar << "\nLexer\n" << bar << endl;
    //for (auto token : c.getTokenStream().getTokens()) {
    //  cout << token->toString() << endl;
    //}

    //cout << bar << "\nParse Tree\n" << bar << endl;
    //cout << c.toPrettyStringTree() << endl;

    //cout << bar << "\nLisp\n" << bar<< endl;
    cymoca::listener::LispPrinter lispPrinter;
    cymoca::ast::Walker walker;
    walker.walk(*c.root(), lispPrinter);
    cout << lispPrinter.get() << endl;
    
  }
  catch (exception &e) {
    cerr << "Unhandled Exception reached the top of main: "
              << e.what() << ", application will now exit" << endl;
    return ERROR_UNHANDLED_EXCEPTION;

  }

  return SUCCESS;

} // main 
/* vim: set et fenc=utf-8 ff=unix sts=0 sw=2 ts=2 : */
