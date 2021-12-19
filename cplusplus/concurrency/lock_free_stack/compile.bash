#!/bin/bash

g++ lock_free_stack.h lock_free_stack_v2.h main.cc -o main --std=c++17 -lpthread -mcx16

echo "compile finished"

if [ $? == 0 ]
then
  ./main
fi
