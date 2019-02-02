[![Build Status](https://travis-ci.org/ZacharyWesterman/libzed.svg?branch=master)](https://travis-ci.org/ZacharyWesterman/libzed)

Currently supports compiling on Linux and Windows only, as those are the only systems I have. ¯\\\_(ツ)\_/¯<br>
It should compile on any flavor of these systems. The only requirements are **make** and **g++** that supports *at least* C++11.
Technically this also does compile on the Raspberry Pi, but I haven't checked it for bugs on that platform..

To compile, run make. Something like `make -j32` will work.<br>
On Windows, you're done. Just copy the DLL into the directory where you want it.<br>
On Linux, type `sudo make install` to install the library and `sudo make uninstall` to remove it.<br>
If you want to test out some code, `make driver` (or `make driver.exe` on Windows) will compile main.cpp into an executable.<br>
Last, `make clean` removes all generated binaries and object files.

[Documentation](https://zacharywesterman.github.io)
