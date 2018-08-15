#include <z/system/console.h>
#include <z/core/binaryStream.h>
#include <z/core/string.h>

const z::core::array< z::core::string<z::utf8> > names =
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
	"RGX_COUNT",
};

const z::core::array< z::core::string<z::utf8> > errors =
{
	"RGX_NO_ERROR",

	"RGX_ERROR",
	"RGX_PARENTH_MISMATCH",
	"RGX_BRACKET_MISMATCH",
	"RGX_BRACE_MISMATCH",
	"RGX_BRACE_INVALID",

	"RGX_LEX_FAIL",
	"RGX_BAD_HEX",
	"RGX_BAD_RANGE",
	"RGX_BAD_COUNT_LOC",
	"RGX_BAD_COUNT_FORM",
	"RGX_BAD_GREED_LOC",
	"RGX_GREED_INEFFECTIVE",

	"RGX_BAD_LOOKAHEAD",
	"RGX_BAD_LOOKBEHIND",

	"RGX_BAD_NEG_FLAG",
	"RGX_BAD_POS_FLAG",

	"RGX_ERR_COUNT"
};

#include <z/util/regex.h>

using namespace z;

void printss(const core::array<util::rgxss>& list, core::outputStream& stream)
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

void printll(const util::rgxll* root, core::outputStream& stream, int level=0)
{
	if (root)
	{
		core::string<utf8> space;
		space.padLeft(" ", level*2).write(stream);

		//print the ID
		core::string<utf8> id = names[root->id()];
		id.padRight(" ",14).write(stream);

		core::string<utf8> range;
		if (root->beg())
		{
			range = "ch["; range += root->beg();
			if (root->end() != root->beg())
			{
				range += L"→"; range += root->end();
			}
			range += "]";
		}
		range.padRight(" ",10).write(stream);

		core::string<utf8> count = "ct{";
		if (root->min() < (size_t)-1)
		{
			count += root->min();
			if (root->max() != root->min())
			{
				count += L"→";
				if (root->max() < (size_t)-1)
					count += root->max();
				else
					count += L"∞";
			}
		}
		else
		{
			count += L"∞";
		}
		count += "}";
		count.padRight(" ", 10).write(stream);

		core::string<utf8> greedy;
		if (!root->greedy())
		{
			greedy = ", not greedy";
		}
		greedy.writeln(stream);

		for (size_t i=0; i<(root->children.length()); i++)
		{
			printll(root->children[i], stream, level+1);
		}
	}
	else
	{
		core::string<utf8> err = "NO NODES CREATED";
		err.writeln(stream);
	}
}

int main()
{
	system::console console;

	const encoding format = utf16;

	core::string<format> pattern = L"[A-Z][\\w ]*?(?=\\.)";
	util::regex<format> rgx (pattern);

	core::binaryStream stream;
	core::string<format> str = "Sentence 1. Sentence 2.";
	str.write(stream);
	stream.seek(0);

	core::string<format> result;
	// printll(rgx.getroot(), console);
	rgx.match(stream) ? (result = "success") : (result = "fail");

	result.writeln(console);
	rgx.errstr().writeln(console);

	result = "Matched {{";
	result += rgx.matched();
	result += "}}";

	result.writeln(console);
	// core::array<util::rgxss> symbols;
	// util::rgxll* root = 0;
	//
	// util::rgxerr error = util::rgxscan(pattern, symbols);
	// if (!error)
	// {
	// 	// printss(symbols, console);
	//
	// 	error = util::rgxlex(symbols, root);
	// 	if (!error)
	// 	{
	// 		printll(root, console);
	// 	}
	// 	else
	// 	{
	// 		core::string<utf8> res = "lex error: ";
	// 		(res+errors[error]).writeln(console);
	// 	}
	// }
	// else
	// {
	// 	core::string<utf8> res = "scan error: ";
	// 	(res+errors[error]).writeln(console);
	// }

	return 0;
}
