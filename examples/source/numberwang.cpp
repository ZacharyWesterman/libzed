//For each integer given as a parameter, this example will print the english representation of that number.

#include <z/all.hpp>

zstring itostr(int val, bool andInt = false)
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

	if (!val)
	{
		return andInt ? "" : "zero";
	}
	else if (val < 13)
	{
		return zstring(andInt ? " and " : "") + ones[val];
	}
	else if (val < 20)
	{
		return zstring(andInt ? " and " : "") + tens[val%10] + "teen";
	}
	else if (val >= 1000000000)
	{
		return zstring(andInt ? " " : "") + itostr(val/1000000000, false) + " billion" + itostr(val%1000000000, true);
	}
	else if (val >= 1000000)
	{
		return zstring(andInt ? " " : "") + itostr(val/1000000, false) + " million" + itostr(val%1000000, true);
	}
	else if (val >= 1000)
	{
		return zstring(andInt ? " " : "") + itostr(val/1000, false) + " thousand" + itostr(val%1000, true);
	}
	else if (val >= 100)
	{
		return zstring(andInt ? " " : "") + itostr(val/100, false) + " hundred" + itostr(val%100, true);
	}
	else
	{
		return zstring(andInt ? " and " : "") + tens[val/10] + "ty" + ((val%10) ? (zstring("-") + ones[val%10]) : "");
	}
}

int main(int argc, char** argv)
{
	z::system::stdout out;

	for (int i=1; i<argc; ++i)
	{
		itostr(int(zstring(argv[i]))).writeln(out);
	}
}
