#include <z/system.h>
#include <z/core.h>
#include <z/file.h>

zstring strtype(const zstring& str)
{
	switch (str.type())
	{
		case z::core::zstr::complex:
			return "complex";
		case z::core::zstr::floating:
			return "float";
		case z::core::zstr::integer:
			return "int";
		default:
			return "string";
	}
}

int main()
{
	z::system::console console;

	zstring item;
	while (true)
	{
		item.read(console);

		strtype(item).writeln(console);
	}

	return 0;
}
