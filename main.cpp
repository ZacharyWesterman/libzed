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
	// double val = 123;
	// double* ptr = new double;
	// std::cout << val << std::endl;
	core::string<utf32> str = "+1.23e4";


	// str = core::string<utf32>(comp.real());
	// core::string<utf32> str = std::complex<double>(0,3.4);
	// core::string<utf32> str = 3.4;

	// str.unPadRight("abc");
	// for (int i=0; i<(int)str.length(); i++)
	std::cout << core::string<utf8>(str).cstring() << std::endl;
	std::cout << str.isFloating() << std::endl;
	// std::cout << (str.isInteger(10) ? "int" : "not int") << std::endl;
	// std::cout << str.length() << std::endl;
	// str = str[1];
	// core::string<utf32> res = "abcee";

	// size_t size = res.length();
	// res.increase();

	// core::string<utf8> tmp;

	// tmp = res + str;
	// core::string<utf32> B = A;
	// core::string<utf8> utfstr = str;
//
	// std::cout << utfstr.cstring() << std::endl;

    return 0;
}
