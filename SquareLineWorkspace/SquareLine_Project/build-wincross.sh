#!/bin/sh
# Cross-build in Linux for Windows

cp  CMakeLists-WinCross.txt  CMakeLists.txt

rm  -r  build
mkdir  -p  build
cd build

cmake  ..  -DCMAKE_BUILD_TYPE=Debug #Release
make  -j2

cd ..

cp  SDL2.dll  build/

./run-wincross.sh