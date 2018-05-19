#include <z/system.h>
#include <z/util.h>
#include <z/core.h>
#include <z/file.h>

using namespace z;


int main()
{
	system::console con;

	util::generic var = core::array<util::generic>{23,"a"};

	// int* var = new int(23);

	// core::string<char> st = 23;

	con.writeln(var.string());

    return 0;
}
