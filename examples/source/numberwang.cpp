//For each integer given as a parameter, this example will print the english representation of that number.

#include <z/all.hpp>

zstring itostr(long val, bool andInt = false)
{
	const char* tens[] = {
		"", "",
		"twen", "thir",
		"four", "fif",
		"six", "seven",
		"eigh", "nine"
	};

	const char* ones[] = {
		"", "one",
		"two", "three",
		"four", "five",
		"six", "seven",
		"eight", "nine",
		"ten", "eleven",
		"twelve"
	};

	const char* powers[] = {
		"quintillion",
		"quadrillion",
		"trillion",
		"billion",
		"million",
		"thousand",
		"hundred"
	};

	const long powvals[] = {
		1000000000000000000,
		1000000000000000,
		1000000000000,
		1000000000,
		1000000,
		1000,
		100
	};

	if (val < 0) return zstring("negative ") + itostr(-val);
	if (!val) return andInt ? "" : "zero";
	if (val < 13) return zstring(andInt ? " and " : "") + ones[val];
	if (val < 20) return zstring(andInt ? " and " : "") + tens[val%10] + "teen";

	for (int i=0; i<7; ++i)
	{
		if (val >= powvals[i])
		{
			return zstring(andInt ? " " : "") + itostr(val/powvals[i], false) + " " + powers[i] + itostr(val%powvals[i], true);
		}
	}

	return zstring(andInt ? " and " : "") + tens[val/10] + "ty" + ((val%10) ? (zstring("-") + ones[val%10]) : "");
}

int main(int argc, char** argv)
{
	z::system::stdout out;

	for (int i=1; i<argc; ++i)
	{
		itostr(long(zstring(argv[i]))).writeln(out);
	}
}
