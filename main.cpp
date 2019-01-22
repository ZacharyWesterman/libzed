#include <z/system.h>
#include <z/core.h>
#include <z/util.h>
#include <z/file.h>

int main()
{
	z::system::console console;

	zstring line = "sample";

	for (auto& chr : line)
	{
		zpath((int)chr).writeln(console);
	}

	return 0;
}
