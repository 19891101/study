#!/bin/bash

g++ decltype.h main.cc -o main --std=c++14

if [ $? == 0 ]
then
  ./main
fi
