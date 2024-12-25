#!/bin/sh
#Build natively on Linux with gcc

cp  CMakeLists-Linux.txt  CMakeLists.txt

rm  -r  build
mkdir  -p  build
cd build

cmake  ..  -DCMAKE_BUILD_TYPE=Debug #Release
make  -j2

cd ..

./run.sh