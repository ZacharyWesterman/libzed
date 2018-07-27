#include <z/system/console.h>
#include <z/util/regex.h>

using namespace z;

const core::array< core::string<utf8> > names =
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
	"RGX_COLUMN",
	"RGX_ASTERISK",
	"RGX_COMMA",
	"RGX_NOT",
	"RGX_BEGIN",
	"RGX_END",

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
	"RGX_HEXDIGIT",
};

const core::array< core::string<utf8> > errors =
{
	"RGX_NO_ERROR",

	"RGX_ERROR",
	"RGX_PARENTH_MISMATCH",
	"RGX_BRACKET_MISMATCH",
	"RGX_BRACE_MISMATCH",
	"RGX_BRACE_INVALID",

	"RGX_COUNT"
};

void print(const core::array<util::rgxss>& list, core::outputStream& stream)
{
	core::string<utf8> res;

	for (size_t i=0; i<list.length(); i++)
	{

		if (list[i].symbol())
		{
			res = names[list[i].id()];
			res.padRight(" ", 14);
			res += "[";
			res += list[i].symbol();
			res += "]";

			res.writeln(stream);
		}
		else
		{
			res = names[list[i].id()];
			res.writeln(stream);
		}
	}
}

int main()
{
	core::string<utf32> pattern = L"(123[0-9][])";
	core::array<util::rgxss> symbols;
	system::console console;

	util::rgxerr error = util::rgxscan(pattern, symbols);

	if (error)
	{
		core::string<utf8> res = "error: ";
		(res+errors[error]).writeln(console);
	}
	else
	{
		print(symbols, console);
	}

	return 0;
}
