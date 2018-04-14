#include <z/system.h>
#include <z/util.h>
#include <z/core.h>
#include <z/file.h>

using namespace z;


int main()
{
	system::console con;

	core::array<util::generic> vals = {g(12345), g("Hello"), g(3.14)};

	util::generic var(vals);

	con.writeln(var.string());

    return 0;
}
