[![Build Status](https://travis-ci.org/ZacharyWesterman/libzed.svg?branch=master)](https://travis-ci.org/ZacharyWesterman/libzed)

This library contains useful classes and functions for quick and simple data manipulation. The idea is that this will help decrease development time of other projects.

Currently supports compiling on Linux and Windows only, as those are the only systems I have. ¯\\\_(ツ)\_/¯<br>
It should compile on any 64 or 32-bit flavor of these systems (including Raspberry Pi!).

The only requirements are **make** and a C++ compiler that supports *at least* C++11 (C++17 or newer preferred).
If **g++ --version** is 5 or above, chances are you're good.<br>

Full build instructions along with make options are detailed on the main page of the [Documentation][docs].

Also of note is that certain classes in this library can be serialized if the
[cereal](https://github.com/USCiLab/cereal) headers are available!
Just make sure they're in the include path and you should be all set.
This is optional, and not necessary to use this library.

[Documentation is here.][docs]

[Look here if you want to contribute.](CONTRIBUTING.md)

[docs]: https://zacharywesterman.github.io
