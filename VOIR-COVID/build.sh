#!/bin/sh
rm -rf build
mkdir build
cd build
cmake ..
make
cd ..
echo
./build/covid
