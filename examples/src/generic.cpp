#include <z/util/generic.hpp>
#include <iostream>

#include <z/core/join.hpp>

int main()
{
//Don't throw error if no generic exists. Just make a dummy executable.
#	ifdef Z_GENERIC_EXISTS
	z::util::generic val1 = {
		"hello",
		5.4,
		z::cplx(1.2,4),
		{ "a", "b" }
	};
	z::util::generic val2 = "test";

	std::cout << val1.typeString() << " : " << val1.toString(true) << std::endl;
	std::cout << val2.typeString() << " : " << val2.toString(true) << std::endl;
#	else

	"ERROR: This program was compiled without support for z::util:generic!"_zs.writeln(std::cout);

#	endif
}
