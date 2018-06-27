// #define Z_USE_NARROW

// #include <z/system.h>
// #include <z/util.h>
// #include <z/core.h>
// #include <z/file.h>

#include <iostream>

#include <z/core/string.h>
using namespace z;

/*template <typename T>
void print(core::inputStream<T>& stream, core::outputStream<char>& con)
{
	// system::console con;

	con.writeln("{");

	Int col = 1;

	stream.seek(0);
	while(!stream.empty())
	{
		core::string<char> pad = stream.get();
		for (Int i=pad.length(); i<5; i++)
			pad += " ";

		con.write(cs("  ")+pad);
		if (!(col % 5))
		{
			con.writeln();
			col = 0;
		}
		col++;
	}

	if (col % 5) con.writeln();

	con.writeln("}");
}*/



int main()
{
	double val = 0.000000000000000123;
	std::cout << val << std::endl;
	core::string<ascii> str = val;
	// core::string<utf32> res = "abcee";

	// size_t size = res.length();
	// res.increase();

	// core::string<utf8> tmp;

	// tmp = res + str;
	// core::string<utf32> B = A;

	std::cout << str.cstring() << std::endl;

    return 0;
}
