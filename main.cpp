#define Z_USE_NARROW

#include <z/system.h>
#include <z/util.h>
#include <z/core.h>
#include <z/file.h>

using namespace z;


int main()
{
	system::console con;

	// util::generic var = cs("1+7i");
	core::string<char> var = "17+12i";

	// core::string<Char> result = var.boolean() ? "true" : "false";

	std::cout << var.complexValue() << std::endl;

	// con.writeln(result.complex());

    return 0;
}
