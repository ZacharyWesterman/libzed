#include <z/system.h>
#include <z/core.h>
#include <z/file.h>

zstring strtype(const zstring& str)
{
	switch (str.type())
	{
		case z::core::zstr::complex:
			return zstring("complex : ")+str.complex();
		case z::core::zstr::floating:
			return zstring("float : ")+str.floating();
		case z::core::zstr::integer:
			return zstring("int : ")+str.integer();
		default:
			return "string";
	}
}

int main()
{
	z::system::console console;

	zstring item;
	while (!console.empty())
	{
		item.read(console);

		strtype(item).writeln(console);
	}

	return 0;
}
