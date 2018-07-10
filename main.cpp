// #define Z_USE_NARROW

// #include <z/system.h>
// #include <z/util.h>
// #include <z/core.h>
// #include <z/file.h>

#include <iostream>

// #include <z/core/array.h>
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
	// core::array<int> A = {1,2,3,4,5,6,7,8,9,10};
	//
	// core::array<int> C = A.remove(5,-4);
	//
	// std::cout << '{';
	// for (size_t i=0; i<C.length(); i++)
	// {
	// 	if (i) std::cout << ',';
	// 	std::cout << C[i];
	// }
	// std::cout << '}' << std::endl;

    return 0;
}
