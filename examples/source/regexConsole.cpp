/*
* A short example to demonstrate stream IO and regex pattern matching.
*/

#include <z/system/console.h>
#include <z/core.h> //for core::memoryStream and core::string
#include <z/util/regex.h>

int main()
{
	z::system::console console;
	zstring pattern;
	z::util::regex regex;

	while(!console.empty())
	{
		//Get regex pattern to match against, exiting if user hits ctrl-D.
		pattern.readln(console);
		if (console.empty() && !pattern.length()) break;

		//If pattern is invalid, print a short description of the error.
		regex.set(pattern);
		if (regex.error())
		{
			regex.errorString().writeln(console);
			continue;
		}

		//Prompt for the string to match to the regex pattern.
		//Note that zstring is equivalent to z::core::string<z::utf32>
		zstring(":").write(console);
		zstring text;
		text.readln(console);

		//Temporary stream to directly access a string's content.
		//This can break stuff if used incorrectly, but using
		//it here is much faster than writing to a binary stream.
		z::core::memoryStream temp(text.wstring(), text.length());
		temp.setFormat(z::utf32);

		//Show what substring the regex matched, if anything.
		if (regex.match(temp))
			(regex.matched() + " matches!").writeln(console);
		else
			zstring("does not match.").writeln(console);
	}

	return 0;
}
