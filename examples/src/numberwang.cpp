//For each integer given as a parameter, this example will print the english representation of that number.

#include <z/all.hpp>
#include <iostream>

int main(int argc, char** argv)
{

	for (int i=1; i<argc; ++i)
	{
		long value = long(zstring(argv[i]));
		z::core::string<z::utf8>::words(value).writeln(std::cout);
	}
}
