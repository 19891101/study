#!/bin/bash

g++ stack.h main.cc -o main --std=c++14

if [ $? == 0 ]
then
  ./main
fi
