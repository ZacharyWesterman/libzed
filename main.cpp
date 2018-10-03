#include <z/system.h>
#include <z/util/dictionary.h>
#include <z/core/timer.h>

#include <z/file/inputStream.h>
#include <z/file/outputStream.h>

#define str z::core::string<z::utf8>

int main()
{
	z::system::console console;
	z::util::dictionary dict;

	// // str("Reading...").write(console);
	z::core::timer timer;
	dict.read("US.dic");
	// z::file::inputStream input ("us_eng.dict");
	// dict.serialIn(input);
	// input.close();
	double wc = dict.wordCount();
	double tm = timer.micros();
	// str("Done.").writeln(console);
	double rt = 0.0;
	if (wc) rt = tm/wc;

	(str("Read ")+wc+" words in "+(tm/1000.0)+"ms (avg "+rt+L" μs/w)").writeln(console);

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

	z::file::outputStream stream ("us_eng.dict");
	dict.serialOut(stream);

	return 0;
}
