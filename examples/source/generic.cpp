#include <z/util/generic.hpp>
#include <iostream>

#include <z/core/join.hpp>

int main()
{
	z::util::generic val1 = {
		"hello",
		5.4,
		z::cplx(1.2,4),
		{ "a", "b" }
	};
	z::util::generic val2 = "test";

	std::cout << val1.typeString() << " : " << val1.toString(true) << std::endl;
	std::cout << val2.typeString() << " : " << val2.toString(true) << std::endl;
}
