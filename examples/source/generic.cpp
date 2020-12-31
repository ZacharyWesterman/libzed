#include <z/util/generic.hpp>
#include <iostream>

#include <z/core/join.hpp>

int main()
{
	z::util::generic val1 = z::util::generic::list {
		"hello",
		5,
		std::complex<double>(1.2,4),
		z::util::generic::list {
			"a",
			"b"
		}
	};
	z::util::generic val2 = "test";

	const char* types[] = {
		"int",
		"float",
		"complex",
		"string",
		"array"
	};

	std::cout << types[val1.type()] << " : " << val1.toString(true) << std::endl;
	std::cout << types[val2.type()] << " : " << val2.toString() << std::endl;
}
