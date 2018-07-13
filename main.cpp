// #define Z_USE_NARROW

// #include <z/system.h>
// #include <z/util.h>
// #include <z/core.h>
// #include <z/file.h>

#include <iostream>

// #include <z/core/array.h>
#include <z/core/string.h>
#include <z/system/console.h>
#include <z/file/stream.h>
using namespace z;

int main()
{
	core::string<ascii> A;
	core::string<ascii> L ("<<"), R (">>");
	file::inputStream input ("test.txt");
	file::outputStream output ("out.txt");

	if (input.good())
	{
		while (!input.empty())
		{
			A.readln(input);
			(L + A + R).writeln(output);
		}
	}
	else
	{
		A = "!! Error reading from stream.";
		A.writeln(output);
	}

    return 0;
}
