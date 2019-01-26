#include <z/system.h>
#include <z/core.h>
#include <z/util.h>
#include <z/file.h>

using z::system::console;
using z::core::array;
using z::core::string;

using z::core::split;
using z::core::join;

int main()
{
	console con;

	zstring input = "the,quick,brown,fox";
	zstring delim = ",";

	auto list = split(input,delim);

	delim = " : ";
	(zstring("{")+join(list,delim)+"}").writeln(con);

	return 0;
}
