#include <z/system.h>
#include <z/util/regex/rgxscan.h>
#include <z/util/regex/rgxlex.h>
#include <z/util/regex/rgxid.h>

const char* ids[] =
{
	"RGX_SYMBOL",
	"RGX_LPAREN",
	"RGX_RPAREN",
	"RGX_LBRACKET",
	"RGX_RBRACKET",
	"RGX_LBRACE",
	"RGX_RBRACE",
	"RGX_DASH",
	"RGX_QUESTION",
	"RGX_QUERY",
	"RGX_GREEDY",
	"RGX_COLUMN",
	"RGX_ASTERISK",
	"RGX_COMMA",
	"RGX_NOT",
	"RGX_BANG",
	"RGX_PREVIOUS",
	"RGX_EQUALS",
	"RGX_PLUS",

	"RGX_BEGIN",
	"RGX_PERIOD",
	"RGX_SPACE",
	"RGX_NOT_SPACE",
	"RGX_ALPHA",
	"RGX_NOT_ALPHA",
	"RGX_DIGIT",
	"RGX_NOT_DIGIT",
	"RGX_ALNUM",
	"RGX_NOT_ALNUM",
	"RGX_WORD",
	"RGX_NOT_WORD",
	"RGX_NEWLINE",
	"RGX_BREAK",
	"RGX_NOT_BREAK",
	"RGX_PUNCT",
	"RGX_NOT_PUNCT",
	"RGX_POS_LOOKAHEAD",
	"RGX_NEG_LOOKAHEAD",
	"RGX_POS_LOOKBEHIND",
	"RGX_NEG_LOOKBEHIND",
	"RGX_POS_FLAG",
	"RGX_NEG_FLAG",
	"RGX_END",
	"RGX_HEXDIGIT",

	"RGX_OR_LIST",
	"RGX_AND_LIST",
	"RGX_COUNT"
};

void printSS(z::core::outputStream& stream, const z::util::rgxss& ss)
{
	zpath s = " ";
	if (ss.id() > z::util::RGX_COUNT) return;
	s += ids[ss.id()];
	if (ss.symbol())
	{
		s += " ";
		s += ss.symbol();
	}
	s.writeln(stream);
}

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

		for (auto& ss : symbols) printSS(stream, ss);

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
