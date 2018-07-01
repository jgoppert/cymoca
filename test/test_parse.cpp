#include <boost/filesystem.hpp>
#include <gtest/gtest.h>
#include <iostream>

#include <cymoca_compiler/Compiler.h>
#include <cymoca_compiler/listener/LispPrinter.h>

using namespace std;
using namespace cymoca;
namespace fs = boost::filesystem;

TEST(ParseTest, BouncingBall) {
  cout << endl;
  fs::path p("../../test/models/BouncingBall.mo");
  ASSERT_TRUE(exists(p));
  ifstream fileStream(p.string());
  Compiler c(fileStream);
  EXPECT_STREQ("Hello World!", "Hello World!");

  listener::LispPrinter listener;
  Walker walker;
  walker.walk(*c.root(), listener);
  cout << listener.get() << endl;
}

// vim: set et fenc=utf-8 ff=unix sts=0 sw=2 ts=2 :
