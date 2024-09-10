#include <z/util/dictionary.hpp>
#include <z/file/inputStream.hpp>
#include <iostream>
#include <fstream>

int main()
{
	z::util::dictionary dict;
	std::ifstream file ("/usr/share/dict/words");

	z::core::timeout time (1000000); //1 second timeout

	std::cout << "Loading..." << std::flush;
	while (!dict.read(file, time))
	{
		std::cout << '.' << std::flush;
		time.reset();
	}

	std::cout << " Loaded " << dict.length() << " words." << std::endl;
}
