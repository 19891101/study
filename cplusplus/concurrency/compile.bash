#!/bin/bash

g++ threadsafe_queue.h hierarchical_mutex.h rw_mutex.h test.cc -o a.out --std=c++17 -lpthread

if [ $? == 0 ]
then
  ./a.out
fi
