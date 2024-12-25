#SquareLine CMake-based board-template to build by CMake or VS-Code or Eclipse

## Prerequisites:
As toolchain, you need CMake and gcc (or `mingw64-gcc` for windows) with `make` (or `ninja`) generator-tools:

### GCC and make:
- On Windows [MinGW](https://www.mingw-w64.org/) contains them, the POSIX+SEH+UCRT-build variant at their [GitHub repository](https://github.com/niXman/mingw-builds-binaries/releases) works fine on Windows 10:
  - Download for example: [x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev0.7z](https://github.com/niXman/mingw-builds-binaries/releases/download/13.2.0-rt_v11-rev0/x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev0.7z)
  - Extract/copy the compressed 7z file's included 'mingw64' folder to C:\  (Can be other folder but we continue with this here.)
  - You need to add the 'C:\mingw64\bin' folder to your PATH environment variable (Unfortunately there's no up-to-date mingw64 installer that sets it for us.)
  (On Windows 10 right-click on This PC, go to Advanced System Settings / Environment Variables, select 'Path' and add "C:\mingw64\bin" to an empty line.)
  - (You can check if the setting works by typing 'mingw32-make' on command line.)
- On Linux type in the Terminal: `sudo apt-get install gcc g++ gdb build-essential`

### CMake
- For Windows go to [CMake download page](https://cmake.org/download/) and select the [CMake MSI-installer binary distribution](https://github.com/Kitware/CMake/releases/download/v3.28.0-rc2/cmake-3.28.0-rc2-windows-x86_64.msi)
  (When asked by the installer, select the option that will take care of setting the PATH variable for the cmake executable.)
- For Linux you just need to get and install CMake from the repository, for example `sudo apt-get install cmake`

### SDL2 development-headers/sources and runtime:
You'll need to have SDL2 development libraries (`libsdl2-dev` package on linux) to build, and SDL2 dynamic runtime libraries (`libSDL2.so`/`SDL2.dll`) to run the example.
For example on Debian Linux you can install libsdl2-dev by command: `sudo apt-get install libsdl2-dev`  (this installs the runtime library too if not present already on the system.)
For Windows executable I included x86_64 part of [SDL2 development-libraries and runtime-libraries](https://github.com/libsdl-org/SDL/releases/download/release-2.28.5/SDL2-devel-2.28.5-mingw.zip) to make it easier to compile for Windows (where using SDL2 by CMake is trickier).

## Building the project:
With `build.sh`/`build.bat` or cmake+make commands you can build and run the example.
(The script `build-wincross.sh` is able to compile a Windows executable in Linux, the prerequisite is installation `gcc-mingw-w64` package by `sudo apt-get install gcc-mingw-w64` command.)
(You can build directly by creating the `build` folder and running `cmake` with different arguments, but the .sh/.bat scripts do it for you automatically. Check these scripts for the commands & arguments used behind the scenes.)

## Running the built code
Scripts `run.sh`/`run.bat` can run the generated executable created in 'build' folder. (`run-wincross.sh` can run the cross-built Windows executable with the help of `wine`)

