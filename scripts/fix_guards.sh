#!/bin/bash
if [ $# == 1 ]
then
  fix=$1
fi

paths="src"

res=`checkguard -r --only guard -p "path -1 | upper | append _" $paths`
if [[ $res != "" ]]
then
  echo "problematic headers: $res"
  if [[ $fix == 1 ]]
  then
    echo "fixing"
    guard2once -r $paths
    once2guard -p "path -1 | upper | append _" -r $paths
  fi
else
  echo "guards ok"
fi
