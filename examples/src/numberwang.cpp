// A simple example of the zstring::words() static method.
// For each integer given as a parameter, this example will print the english representation of that number.

#include <iostream>
#include <z/core/string.hpp>

int main(int argc, char **argv) {

	for (int i = 1; i < argc; ++i) {
		auto value = (long)zstring(argv[i]);
		zstring::words(value).writeln(std::cout);
	}
}
