#include <z/core/string.h>
#include <z/system/console.h>

int main()
{
	z::system::console console;

	z::core::string<z::ascii> S = "1234";

	S.writeln(console);

	return S.length();
}
