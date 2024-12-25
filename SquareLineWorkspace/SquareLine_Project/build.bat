rem Build on native Windows with gcc of MinGW

copy  CMakeLists-Windows.txt  CMakeLists.txt

rd  build  /S  /Q
md  build
cd  build

cmake  -G "MinGW Makefiles"  ..  -DCMAKE_BUILD_TYPE=Debug
mingw32-make  -j2

cd  ..

copy  SDL2.dll  build

run.bat

