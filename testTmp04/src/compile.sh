#!/bin/bash
export PATH=/opt/gcc/gcc-4.7.3:$PATH
export LD_LIBRARY_PATH=/opt/gcc/gcc-4.7.3:/opt/gcc/gcc-4.7.3/lib64:$LD_LIBRARY_PATH
g++ -g -Xlinker -rpath=/opt/glibc/lib -Xlinker -I/opt/glibc/lib/ld-2.17.so testTmp04.cpp
