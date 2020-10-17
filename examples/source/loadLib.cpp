//Example to load symbols from a dynamic library.

#include <iostream>
#include <z/file/library.hpp>

int main()
{
	z::file::library lib;
	if (!lib.load("./dynamicLib.so"))
	{
		std::cout << "Unable to load lib!" << std::endl;
		return -1;
	}

	//Note by default, this function signature is assumed to be "void test() {...}".
	//If a function has a different signature you will have to cast it.
	auto func = lib.function("test");
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
