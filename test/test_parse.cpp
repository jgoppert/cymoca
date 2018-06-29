#include <iostream>
#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <vector>

#include <cymoca_compiler/Compiler.h>
#include <cymoca_compiler/listener/LispPrinter.h>

using namespace boost::filesystem;
using namespace cymoca;

TEST(ParseTest, BouncingBall) {
  std::cout << std::endl;
  path p("../../test/models/BouncingBall.mo");
  ASSERT_TRUE(exists(p));
  std::ifstream fileStream(p.string());
  Compiler c(fileStream);
  //std::cout << s<< std::endl;
  EXPECT_STREQ("Hello World!", "Hello World!");

  listener::LispPrinter listener;
  Walker walker;
  walker.walk(*c.getRoot(), listener);
  std::cout << listener.get() << std::endl;
}