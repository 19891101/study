#!/bin/bash

g++ thread_pool.h main.cc -o main --std=c++14 -lpthread

echo "compile finished"

if [ $? == 0 ]
then
  ./main
fi
