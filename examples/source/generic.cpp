#include <z/util/generic.hpp>
#include <iostream>

#include <z/core/join.hpp>

int main()
{
	z::util::generic val1 = z::util::generic::list {
		"hello",
		5.4,
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

	// val2 = int(3);

	std::cout << types[val1.type()] << " : " << val1.toString(true) << std::endl;
	// std::cout << types[val2.type()] << " : " << int(val2) << std::endl;
}
