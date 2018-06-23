#include <iostream>
#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <cymoca_compiler/Compiler.h>
#include <vector>

#include <casadi/casadi.hpp>


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


TEST(CasadiTest, Simple) {
  std::cout << std::endl;
  auto x = casadi::SX::sym("x");
  auto y = x*2;
  auto f = casadi::Function(
      "f",
      std::vector<casadi::SX>({x}),
      std::vector<casadi::SX>({y}),
      std::vector<std::string>({"x"}),
      std::vector<std::string>({"y"})
  );
  std::cout << "casadi test complete" << std::endl;
  EXPECT_STREQ("Hello World!", "Hello World!");
}
