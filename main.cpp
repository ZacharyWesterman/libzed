#include <z/system.h>
#include <z/core.h>
#include <z/util.h>
#include <z/file.h>

using z::system::console;
using z::core::array;
using z::core::string;

using z::core::split;

int main()
{
	console con;

	string<z::utf32> input = "the,quick,brown,fox";
	string<z::utf32> delim = ",";

	input.substr(0,3).writeln(con);

	auto list = split(input,delim);
	zpath(list.length()).writeln(con);

	for (auto& item : list)
	{
		zpath(item+"::").writeln(con);
	}

	return 0;
}
