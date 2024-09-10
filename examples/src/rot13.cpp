#include <z/all.hpp>
#include <iostream>

int main(int argc, char** argv)
{
	zstring from = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	zstring to   = "nopqrstuvwxyzabcdefghijklmNOPQRSTUVWXYZABCDEFGHIJKLM";
	for (int i=1; i<argc; ++i)
	{
		(zstring(argv[i]) + " ").cipher(from, to).write(std::cout);
	}
	zstring().writeln(std::cout);
}
