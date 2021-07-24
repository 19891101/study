#!/bin/bash

g++ enable_if.h main.cc -o main --std=c++14

if [ $? == 0 ]
then
  ./main
fi
