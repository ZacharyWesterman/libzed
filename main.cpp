// #define Z_USE_NARROW

#include <z/system.h>
// #include <z/util.h>
// #include <cstdint>

#include <z/core.h>
// #include <z/file.h>

// #include <z/core/array.h>
// #include <z/core/serializable.h>
// #include <z/core/string.h>
// #include <z/system/console.h>
// #include <z/core/binaryStream.h>

using z::core::string;
using z::system::console;

int main()
{
	string<z::utf8> A = "Hello, this is a test. 1234test, _test";

	console output;

	(A.camel()).writeln(output);

    return 0;
}
