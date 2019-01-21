#include <z/system.h>
#include <z/core.h>
#include <z/util.h>
#include <z/file.h>

int main()
{
	z::system::console console;

	zstring line;
	while (!console.empty())
	{
		zpath("input: ").write(console);
		line.readln(console);
		line.writeln(console);
	}

	return 0;
}
