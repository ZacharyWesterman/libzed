#include <z/system.h>
#include <z/core.h>
#include <z/file.h>

int main()
{
	z::system::console console;

	z::core::string<z::utf8> str = "this is a memory stream test!";
	z::core::memoryStream stream (str.cstring(), 29);

	z::core::string<z::utf8> repl = "prebus";
	stream.seek(10);

	// std::cout << (void*)buffer << std::endl;

	// stream.put('G');
	repl.write(stream);

	str.writeln(console);

	return 0;
}
