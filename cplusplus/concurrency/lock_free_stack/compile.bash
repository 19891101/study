#!/bin/bash

g++ lock_free_stack.h main.cc -o main --std=c++14 -lpthread

echo "compile finished"

if [ $? == 0 ]
then
  ./main
fi
