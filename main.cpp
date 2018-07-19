// #define Z_USE_NARROW

// #include <z/system.h>
// #include <z/util.h>
// #include <z/core.h>
// #include <z/file.h>

// #include <z/core/array.h>
#include <z/core/string.h>
// #include <z/core/binaryStream.h>
#include <z/system/console.h>
#include <z/file/stream.h>
using namespace z;

template <typename T>
size_t getSize(const T& object)
{
	return sizeof(object);
}

template <>
size_t getSize(const core::sizable& object)
{
	return object.size();
}

int main()
{
	core::string<utf8> A = "12345";
	core::string<utf8> B = getSize(A);

	system::console output;

	(B).writeln(output);

    return 0;
}
