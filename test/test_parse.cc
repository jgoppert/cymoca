#include <gtest/gtest.h>
#include <experimental/filesystem>
#include <iostream>
#include "test_config.h"

#include <cymoca/ast/listener/lisp.h>
#include "cymoca/compiler.h"

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
  ast::listener::Lisp printer;
  ast::Walker walker;
  walker.walk(*c.root(), printer);
  std::cout << printer.get() << std::endl;
}
