[![Build Status](https://travis-ci.org/ZacharyWesterman/libzed.svg?branch=master)](https://travis-ci.org/ZacharyWesterman/libzed)

This library contains useful classes and functions for quick and simple data manipulation. The idea is that this will help decrease development time of other projects.

Currently supports compiling on Linux and Windows only, as those are the only systems I have. ¯\\\_(ツ)\_/¯<br>
It should compile on any flavor of these systems.

The only requirements are **make** and a C++ compiler that supports *at least* C++11.
If **g++ --version** is 5 or above, chances are you're good.<br>
This also does compile on the Raspberry Pi, but I haven't checked it for bugs.<br>
Since this library doesn't really mess with hardware though, it should behave fine on a 32-bit system as it does 64-bit.

To compile, run `make`.<br>
On Windows, you're done. Just copy the DLL into the directory where you want it.<br>
On Linux, type `sudo make install` to install the library and headers, and `sudo make uninstall` to remove it.<br>
If you want to test out some code, `make examples` will run the makefile in the examples/ subdirectory.<br>
Last, `make clean` removes all generated binaries and object files.

Full build instructions along with make options are detailed on the main page of the [Documentation][docs].

I would also like to note that certain classes in this library can be serialized if the
[cereal](https://github.com/USCiLab/cereal) headers are available!
Just make sure they're in the include path and you should be all set.
This is optional, and not necessary to use this library.

[Documentation is here.][docs]

[Look here if you want to contribute.](CONTRIBUTING.md)

[docs]: https://zacharywesterman.github.io
