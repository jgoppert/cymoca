#!/bin/bash
if [ $# != 0 ]
then
  echo """
  usage: $0
  """
  exit 1
fi
apply=$1

files=`find . -regex "./\(src\|test\).*\.\(cpp\|c\|h\|cc\|hpp\)"`
clang-tidy-6.0 $files
