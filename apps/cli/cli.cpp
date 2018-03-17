#include <iostream>
#include <boost/filesystem.hpp>
#include <cymoca/Compiler.h>

using namespace boost::filesystem;

int main(int argc, char * argv[]) {
  path p("../test/models/Simple.mo");
  assert(exists(p));
  std::ifstream fileStream(p.string());
  cymoca::Compiler c;
  c.compile(fileStream);
  c.printXML(std::cout);
  //std::cout << s<< std::endl;
};
