#include <z/system.h>
#include <z/util/dictionary.h>
#include <z/core/timer.h>

#define str z::core::string<z::utf32>

int main()
{
	z::system::console console;
	z::util::dictionary dict;

	// str("Reading...").write(console);
	z::core::timer timer;
	dict.read("US.dic");
	double wc = dict.wordCount();
	double tm = timer.micros();
	// str("Done.").writeln(console);

	(str("Read ")+wc+" words in "+(tm/1000.0)+"ms (avg "+(tm/wc)+L" μs/w)").writeln(console);

	// z::system::pause(1000);


	str search = "}";

	size_t count = 50000;

	timer.reset();
	for (size_t i=0; i<count; i++)
	{
		dict.isWord(search);
	}

	str result = ((double)timer.micros() / (double)count);

	(result + L"μs avg. search").writeln(console);


	// s(z::util::word("I") < z::util::word('a')).writeln(console);

	return 0;
}
