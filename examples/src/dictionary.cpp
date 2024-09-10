#include <z/util/dictionary.hpp>
#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
	z::util::dictionary dict;
	std::fstream file ("/usr/share/dict/words");

	z::core::timeout time (1000000); //1 second timeout

	std::cout << "Loading..." << std::flush;
	while (!dict.read(file, time, true))
	{
		std::cout << '.' << std::flush;
		time.reset();
	}

	std::cout << " Loaded " << dict.length() << " words." << std::endl;
	
	if (argc > 1)
	{
		std::cout << dict.isWord(argv[1]) << std::endl;
	}
}
