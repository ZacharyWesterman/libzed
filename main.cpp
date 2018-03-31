#include <z/system.h>
#include <z/util.h>
#include <z/core.h>
#include <z/file.h>

using namespace z;

int main()
{
	system::cpuid cpu;
	system::console console;

	util::regex<char> regex ("\\s{2,5}");

	if (regex.bad())
		console.writeln("Invalid regex formatting.");

	core::stringStream<char> stream (cs("Hello, sir.   This is an example of bad   spacing."));

	Int found = regex.search(stream);

	if (found > 0)
	{
		auto msg = cs("Found ")+regex.foundLen()+" characters at index ";
		msg += cs(regex.foundAt())+" in input. (\"";
		msg += regex.foundString()+"\")";
		console.writeln(msg);
	}
	else
		console.writeln("No string found matching regex.");

    return 0;
}
