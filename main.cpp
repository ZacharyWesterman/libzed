#include <z/system.h>
#include <z/util.h>
#include <z/core.h>

using namespace z;

int main()
{
	system::console console;
	util::regex<char> regex("a~[cr]");

	if (regex.bad())
		console.writeln("Invalid regex formatting.");

	core::stringStream<char> stream("Zachary Dewane");

	Int found = regex.search(stream);

	if (found > 0)
	{
		console.writeln(cs("Found ")+regex.foundLen()+" characters at index "+regex.foundAt()+" in input.");
	}
	else
		console.writeln("No string found matching regex.");

    return 0;
}



