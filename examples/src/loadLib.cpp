//Example to load symbols from a dynamic library.

#include <iostream>
#include <z/file/library.hpp>
#include <z/file/dirname.hpp>
#include <z/file/executable.hpp>

int main()
{
	z::file::library lib;

	//assume library is in same dir as executable, not necessarily the working dir.
	auto libName = z::file::dirname(z::file::executable()) + "/dynamicLib";
	if (!lib.load(libName))
	{
		std::cout << "Unable to load lib!" << std::endl;
		return -1;
	}

	//Load a symbol, assuming it to be a function that returns void and has no parameters.
	auto func = lib.function<void()>("test");
	if (!func)
	{
		std::cout << "Unable to load symbol \"test\"." << std::endl;
		return -1;
	}

	std::cout << "Loaded symbol \"test\" as function. Result:" << std::endl;
	func();

	//It's a good habit to unload the library when we no longer need it, and to null out any symbols.
	lib.unload();
	func = nullptr;

	return 0;
}
