// Example to load symbols from a dynamic library.
// This executable loads a DLL/SO file whose name can be be unknown until run-time.
// See the dynamicLib.cpp example for how to define symbols in a library.

#include <iostream>
#include <z/file/execdir.hpp>
#include <z/file/library.hpp>

int main() {
	z::file::library lib;

	// Assume library is in same dir as executable, not necessarily the working dir.
	// Note that you do not have to specify the file extension of the library;
	// by default, the correct extension will be appended based on your OS.
	auto libName = z::file::execdir() + "/dynamicLib";
	if (!lib.load(libName)) {
		std::cout << "Unable to load lib!" << std::endl;
		return -1;
	}

	// Load a symbol, assuming it to be a function that returns void and has no parameters.
	auto func = lib.function<void()>("test");
	if (!func) {
		std::cout << "Unable to load symbol \"test\"." << std::endl;
		return -1;
	}

	std::cout << "Loaded symbol \"test\" as function. Result:" << std::endl;
	func();

	// It's a good habit to unload the library when we no longer need it.
	lib.unload();

	return 0;
}
