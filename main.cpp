// #define Z_USE_NARROW

// #include <z/system.h>
// #include <z/util.h>
// #include <z/core.h>
// #include <z/file.h>

#include <iostream>

// #include <z/core/array.h>
#include <z/core/string.h>
#include <z/system/console.h>
using namespace z;

int main()
{
	core::string<ascii> A;
	core::string<ascii> L ("<<"), R (">>");
	system::console con;

	A.readln(con);
	// std::cout << ';' << A.length() << ';';
	// core::string<ascii> C = A + B;
	(L + A + R).writeln(con);

	// A.writeln(B);

    return 0;
}
