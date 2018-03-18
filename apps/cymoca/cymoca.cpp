#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <cymoca_compiler/Compiler.h>
#include <cymoca_compiler/version.h>

using namespace boost::filesystem;
namespace po = boost::program_options;
using namespace std;

int main(int argc, const char * argv[]) {
  po::positional_options_description p;
  p.add("input-file", -1);
  int opt;

  // group of options allowed only on command line
  po::options_description generic("Generic options");
  generic.add_options()
      ("version,v", "print version string")
      ("help,h", "produce help message")
      ;

  // group of options allowed on command line and
  // in config file
  po::options_description config("Configuration");
  config.add_options()
      ("optimization", po::value<int>(&opt)->default_value(10),
       "optimization level")
      ("include-path,I", po::value<string>()->composing(),
       "include path")
      ;

  // hidden options, will be allowed both on command line
  // and in config file, but will not be shown to the user
  po::options_description hidden("Hidden options");
  hidden.add_options()
      ("input-file", po::value<vector<string>>(), "input file")
      ;

  po::options_description cmdline_options;
  cmdline_options.add(generic).add(config).add(hidden);

  po::options_description config_file_options;
  config_file_options.add(config).add(hidden);

  po::options_description visible;
  visible.add(generic).add(config);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
      options(visible).positional(p).run(), vm);
  po::notify(vm);

  if (vm.count("help")) {
    cout << "Cymoca Modelica compiler " << VERSION << std::endl;
    cout << "usage: cymoca [options] model.mo" << endl;
    cout << visible << endl;
    return 1;
  }

  //path p("../test/models/Simple.mo");
  //assert(exists(p));
  //std::ifstream fileStream(p.string());
  //cymoca::Compiler c;
  //c.compile(fileStream);
  //c.printXML(std::cout);
  //std::cout << s<< std::endl;
};


/* vim: set et fenc=utf-8 ff=unix sts=0 sw=4 ts=4 : */
