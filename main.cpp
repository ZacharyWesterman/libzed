#include <z/system.h>
#include <z/core.h>

int main()
{
	z::system::console console;
	z::core::string<z::utf8> message = "Hello, world!";

	message.writeln(console);

	return 0;
}
