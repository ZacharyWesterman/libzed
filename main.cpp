#include <z/system.h>
#include <z/util.h>
#include <z/core.h>
#include <z/file.h>

#define str z::core::string<z::utf8>

int main()
{
	z::system::console console;
	str(sizeof(size_t)).writeln(console);
	// z::util::dictionary dict;
	//
	// str("Reading").write(console);
	// z::system::pause(1000);
	// z::core::timer timer;
	// z::core::timeout time (100000);
	// z::file::inputStream input ("US.dic");
	//
	// do
	// {
	// 	str('.').write(console);
	// 	time.reset();
	// 	z::system::pause(10);
	// }
	// while(!dict.read(input,time));
	//
	// str().writeln(console);
	// // z::file::inputStream input ("us_eng.dict");
	// // dict.serialIn(input);
	// // input.close();
	// double wc = dict.wordCount();
	// double tm = timer.micros();
	// // str("Done.").writeln(console);
	// double rt = 0.0;
	// if (wc) rt = tm/wc;
	// z::system::pause(10);
	//
	// (str("Read ")+wc+" words in "+(tm/1000.0)+"ms (avg "+rt+L" μs/w)").writeln(console);
	//
	//
	//
	// str search = "}";
	//
	// size_t count = 50000;
	//
	// timer.reset();
	// for (size_t i=0; i<count; i++)
	// {
	// 	dict.isWord(search);
	// }
	//
	// str result = ((double)timer.micros() / (double)count);
	//
	// (result + L"μs avg. search").writeln(console);

	// z::file::outputStream stream ("us_eng.dict");
	// dict.serialOut(stream);

	return 0;
}
