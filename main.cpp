#include <z/system.h>
#include <z/core.h>
#include <z/util.h>
#include <z/file.h>

int main()
{
	z::system::console console;

	z::file::outputStream out ("temp", z::ascii);
	z::util::generic* g1 = new z::util::genericInt(260);
	z::core::serialOut(g1,out);

	out.close();
	z::util::generic* g2 = NULL;

	z::file::inputStream in ("temp");
	z::core::serialIn(g2, in);

	(g2->typeString() + " : " + g2->string()).writeln(console);

	return 0;
}
