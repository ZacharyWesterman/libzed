[![Build Status](https://travis-ci.org/ZacharyWesterman/libzed.svg?branch=master)](https://travis-ci.org/ZacharyWesterman/libzed)

Currently supports compiling on Linux and Windows only, as those are the only systems I have. ¯\\\_(ツ)\_/¯<br>
It should compile on any flavor of these OS's. The only requirements are **make** and **g++** that supports *at least* C++11.
Technically this also does compile on the Raspberry Pi, but I haven't checked it for bugs on that platform..

To compile, run make. Something like `make -j32` will work.<br>
On Windows, you're done. Just copy the DLL into the directory where you want it.<br>
On Linux, type `sudo make install` to install the library and `sudo make uninstall` to remove it.<br>
If you want to test out some code, `make driver` (or `make driver.exe` on Windows) will compile main.cpp into an executable.<br>
Last, `make clean` removes all generated binaries and object files.

If you want to add support for more operating systems, or any new features, bugfixes, etc. you can submit a merge request. If you have any questions you can email me at westerman.zachary@gmail.com

|Coding style / rules|
|:-----|
|Indent is a single *hard* tab. Only deviants use spaces.|
|All public entities must use `camelCaseLikeThis`. Please use reasonable and descriptive names.|
|All non-static functions, classes, enums, etc. must be under an appropriate namespace under the `z` namespace.|
|I use Doxygen to document my code. All public classes, structs, enums, functions, etc. should have some documentation.|
|Private or static entities do not *have to* have any consistency or documentation.|

If you do submit changes, please follow my code style. No spaghetti code please, reading it physically hurts me.
