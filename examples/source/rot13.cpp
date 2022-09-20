#include <z/all.hpp>

int main(int argc, char** argv)
{
	z::system::stdout console;
	zstring from = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	zstring to   = "nopqrstuvwxyzabcdefghijklmNOPQRSTUVWXYZABCDEFGHIJKLM";
	for (int i=1; i<argc; ++i)
	{
		(zstring(argv[i]) + " ").cipher(from, to).write(console);
	}
	zstring().writeln(console);
}
