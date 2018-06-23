#include <iostream>
#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <cymoca_compiler/Compiler.h>
#include <vector>


using namespace boost::filesystem;

TEST(CompilerTest, Simple) {
  std::cout << std::endl;
  path p("../../test/models/BouncingBall.mo");
  ASSERT_TRUE(exists(p));
  std::ifstream fileStream(p.string());
  cymoca::Compiler c(fileStream);
  //std::cout << s<< std::endl;
  EXPECT_STREQ("Hello World!", "Hello World!");
}