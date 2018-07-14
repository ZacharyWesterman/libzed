// #define Z_USE_NARROW

// #include <z/system.h>
// #include <z/util.h>
// #include <z/core.h>
// #include <z/file.h>

#include <iostream>

// #include <z/core/array.h>
#include <z/core/string.h>
// #include <z/core/binaryStream.h>
#include <z/system/console.h>
#include <z/file/stream.h>
using namespace z;

int main()
{
	core::string<utf8> A;
	core::string<utf8> L ("<<"), R (">>");
	file::inputStream input ("out.txt");
	// file::outputStream output ("out.txt");
	system::console output;

	A.serialIn(input);
	(L+A+R).writeln(output);

    return 0;
}
