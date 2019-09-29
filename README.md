[![Build Status](https://travis-ci.org/ZacharyWesterman/libzed.svg?branch=master)](https://travis-ci.org/ZacharyWesterman/libzed)

Currently supports compiling on Linux and Windows only, as those are the only systems I have. ¯\\\_(ツ)\_/¯<br>
It should compile on any flavor of these systems.<br>
The only requirements are **make** and a **g++** version that supports *at least* C++11.<br>
Technically you could use another C++ compiler, but I use **g++**.<br>
This also does compile on the Raspberry Pi, but I haven't checked it for bugs.<br>
Since this library doesn't really mess with hardware though, I would expect it to behave just like x86 linux.

To compile, run make. Something like `make -j32` will work.<br>
On Windows, you're done. Just copy the DLL into the directory where you want it.<br>
On Linux, type `sudo make install` to install the library and `sudo make uninstall` to remove it.<br>
If you want to test out some code, `make examples` will run the makefile in the examples/ subdirectory.<br>
Last, `make clean` removes all generated binaries and object files.

[Documentation](https://zacharywesterman.github.io)
