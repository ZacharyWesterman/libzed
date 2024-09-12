#include <z/util/dictionary.hpp>
#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
	z::util::dictionary dict;
	std::fstream file ("/usr/share/dict/words");

	std::cout << "Loading..." << std::flush;

	//Load the dictionary, pausing every 1 second to do other processes
	while (!dict.read(file, 1000000, true))
	{
		std::cout << '.' << std::flush;
	}

	std::cout << " Loaded " << dict.length() << " words." << std::endl;
	
	if (argc > 1)
	{
		(zstring(argv[1]) + " is " + (dict.isWord(argv[1]) ? "" : "not ") + "a word.").writeln(std::cout);
	}
}
