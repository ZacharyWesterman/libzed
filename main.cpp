#include <z/system.h>
#include <z/util/regex/rgxscan.h>
#include <z/util/regex/rgxlex.h>

int main()
{
	z::system::console stream;
	zstring pattern;

	while(!stream.empty())
	{
		pattern.readln(stream);
		if (!pattern.length()) continue;

		z::core::array<z::util::rgxss> symbols;
		int err = z::util::rgxscan(pattern,symbols);
		if (err)
		{
			(zpath("Scan error ")+err).writeln(stream);
			continue;
		}

		size_t position = 0;
		z::util::rgx::compound* root = NULL;
		err = z::util::rgxlex(symbols, &root, position);
		if (err)
		{
			(zpath("Lex error ")+err).writeln(stream);
			continue;
		}

		zpath("Valid: ").writeln(stream);
#		ifdef DEBUG
		root->print(stream);
#		endif
	}

	return 0;
}
