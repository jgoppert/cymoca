#include <iostream>
#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <cymoca_compiler/Compiler.h>

using namespace boost::filesystem;

TEST(ComplilerTest, Simple) {

  path p("../../test/models/Simple.mo");
  ASSERT_TRUE(exists(p));
  std::ifstream fileStream(p.string());
  cymoca::Compiler c(fileStream);
  c.printXML(std::cout);
  //std::cout << s<< std::endl;
  EXPECT_STREQ("Hello World!", "Hello World!");
}
