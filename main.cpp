#include <z/system/console.h>
#include <z/core/string.h>
#include <z/file.h>

#define s(v) z::core::string<z::utf8>(v)

int main()
{
	z::system::console console;

	double value = 1;

	z::file::outputStream out ("data");
	s(value).writeln(console);
	z::core::serialOut(value, out);

	value = 0;
	out.close();
	z::file::inputStream in ("data");
	z::core::serialIn(value, in);
	s(value).writeln(console);


	return 0;
}
