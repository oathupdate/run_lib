#!/bin/bash

cd `dirname $0`

mkdir build
cd build
#cmake -D CMAKE_INSTALL_PREFIX=/home/ivar/why/run/build/why ..
cmake3 ..
make
