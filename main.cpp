#include <z/system.h>
#include <z/core.h>
#include <z/util.h>
#include <z/file.h>

int main()
{
	z::system::console console;

	z::core::array<int> list = {1,2,4,8,16,32};

	for (auto& num : list)
	{
		zpath(num).writeln(console);
	}

	return 0;
}
