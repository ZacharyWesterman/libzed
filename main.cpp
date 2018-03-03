#include <z/system.h>

using namespace z;

int main()
{
	system::console console;

	auto input = console.read('\n');

	console.writeln(cs("input = ") + input);

    return 0;
}



