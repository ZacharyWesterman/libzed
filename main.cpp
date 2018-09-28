#include <z/system/console.h>
#include <z/util/dictionary.h>

#define s(x) z::core::string<z::utf8>(x)

int main()
{
	z::system::console console;
	z::util::dictionary dict;

	s("Reading...").write(console);
	dict.read("US.dic");
	s("Done.").writeln(console);

	s(dict.isWord("i")).writeln(console);
	// dict.print(0, 50);



	return 0;
}
