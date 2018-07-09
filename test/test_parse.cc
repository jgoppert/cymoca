#include <gtest/gtest.h>
#include <experimental/filesystem>
#include <iostream>
#include "test_config.h"

#include "cymoca/compiler.h"
//#include <cymoca/listener/LispPrinter.h>

using namespace cymoca;
namespace fs = std::experimental::filesystem;

TEST(ParseTest, BouncingBall) {
  std::cout << std::endl;
  fs::path p =
      fs::path(cymoca::test::path) / fs::path("models/BouncingBall.mo");
  std::cout << "path: " << p << std::endl;
  ASSERT_TRUE(exists(p));
  std::ifstream fileStream(p.string());
  Compiler c(fileStream);
  EXPECT_STREQ("Hello World!", "Hello World!");

  // listener::LispPrinter listener;
  // Walker walker;
  // walker.walk(*c.root(), listener);
  // std::cout << listener.get() << endl;
}
