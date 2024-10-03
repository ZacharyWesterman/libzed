// Example for defining symbols in a dynamic library.
// This file gets compiled into a DLL/SO file that can be loaded into a program dynamically at run-time.
// See the loadLib.cpp example for how to load it.

#include <iostream>

// `extern "C"` is required to expose the interface without C++'s name mangling getting in the way.
extern "C" {
void test() {
	std::cout << "In Dynamic Library.\n";
}
}
