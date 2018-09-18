#include <z/system/console.h>
#include <z/util/generic.h>

int main()
{
	z::system::console console;

	z::util::generic* foo = new z::util::genericComplex(std::complex<double>(123,-456));

	auto type = foo->typeString();
	auto value = foo->string();

	type.writeln(console);
	value.writeln(console);

	return 0;
}
