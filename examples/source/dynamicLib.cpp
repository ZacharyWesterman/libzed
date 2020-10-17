//Example for defining symbols in a dynamic library.

#include <iostream>

extern "C"
{
	void test()
	{
		std::cout << "In Dynamic Library.\n";
	}
}
