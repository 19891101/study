#!/bin/bash

g++ variadic.h main.cc -o main --std=c++14

if [ $? == 0 ]
then
  ./main
fi
