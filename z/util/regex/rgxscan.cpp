#include "rgxscan.h"

#include "rgxid.h"
#include "rgxsesc.h"

#include <z/core/charFunctions.h>

namespace z
{
	namespace util
	{
		rgxerr rgxscan(const zstring& pattern, core::array<rgxss>& output)
		{
			bool inOr = false;
			bool startOr = false;
			bool escaped = false;
			bool orForward = false;
			bool inBrace = false;
			int paren = 0;
			int amount = 0;
			bool canGreedy = false;

			for (size_t i=0; i<pattern.length(); ++i)
			{
				auto ch = pattern[i];

				if (inBrace)
				{
					if (ch == '}')
					{
						if (pattern[i-1] != ',')
							output.add(rgxss(RGX_COUNT, amount));
						output.add(rgxss(RGX_RBRACE));
						inBrace = false;
						canGreedy = true;
					}
					else if (core::isNumeric(ch))
					{
						amount = (amount*10)+core::numeralValue(ch);
					}
					else if (ch == ',')
					{
						output.add(rgxss(RGX_COUNT, amount));
						output.add(rgxss(RGX_COMMA));
						amount = 0;
					}
					continue;
				}

				if (escaped)
				{
					output.add(rgxsesc(ch));
					escaped = false;
					continue;
				}
				else if (ch == '\\')
				{
					escaped = true;
					continue;
				}

				if (inOr)
				{
					if (startOr)
					{
						orForward = !(ch == '^');
						if (ch == '^')
							output.add(rgxss(RGX_NOT));
						else
							output.add(rgxss(RGX_SYMBOL, ch));
						startOr = false;
					}
					else
					{
						if (ch == ']')
						{
							output.add(rgxss(RGX_RBRACKET));
							inOr = startOr = false;
						}
						else if ((ch == '-') && orForward && (pattern[i-1] != '[') && (pattern[i+1] != ']'))
						{
							output.add(rgxss(RGX_DASH));
						}
						else
						{
							output.add(rgxss(RGX_SYMBOL,ch));
						}
						orForward = true;
					}
					continue;
				}

				if (ch == '[')
				{
					inOr = startOr = true;
					output.add(rgxss(RGX_LBRACKET));
					continue;
				}

				if (ch == '(')
				{
					if (pattern[i+1] == '?')
					{
						auto ch2 = pattern[i+2];
						if (ch2 == '<')
						{
							auto ch3 = pattern[i+3];
							if (ch3 == '=')
							{
								i += 2;
								output.add(rgxss(RGX_POS_LOOKBEHIND));
							}
							else if (ch3 == '!')
							{
								i += 2;
								output.add(rgxss(RGX_NEG_LOOKBEHIND));
							}
							else
							{
								output.add(rgxss(RGX_POS_FLAG));
							}
						}
						else if (ch2 == '=')
						{
							++i;
							output.add(rgxss(RGX_POS_LOOKAHEAD));
						}
						else if (ch2 == '!')
						{
							++i;
							output.add(rgxss(RGX_NEG_LOOKAHEAD));
						}
						else if (ch2 == '-')
						{
							++i;
							output.add(rgxss(RGX_NEG_FLAG));
						}
						else
						{
							output.add(rgxss(RGX_POS_FLAG));
						}
						++i;
					}
					else
					{
						output.add(rgxss(RGX_LPAREN));
					}
					++paren;
					continue;
				}

				if (ch == '{')
				{
					if (pattern[i+1] == '}')
					{
						output.add(rgxss(RGX_SYMBOL,'{'));
						output.add(rgxss(RGX_SYMBOL,'}'));
						++i;
						continue;
					}
					inBrace = true;
					output.add(rgxss(RGX_LBRACE));
					continue;
				}

				if (ch == ')')
				{
					if (!paren) return RGX_PARENTH_MISMATCH;
					--paren;
					output.add(rgxss(RGX_RPAREN));
					continue;
				}

				if (ch == '*')
				{
					canGreedy = true;
					output.add(rgxss(RGX_ASTERISK));
					continue;
				}
				if (ch == '+')
				{
					canGreedy = true;
					output.add(rgxss(RGX_PLUS));
					continue;
				}
				if (ch == '?')
				{
					output.add(rgxss(canGreedy?RGX_GREEDY:RGX_QUESTION));
				}

				if (ch == ']') return RGX_BRACKET_MISMATCH;
				else if (ch == '}') return RGX_BRACE_MISMATCH;
				else if (ch == '.') output.add(rgxss(RGX_PERIOD));
				else if (ch == '|') output.add(rgxss(RGX_COLUMN));
				else if (ch == '^') output.add(rgxss(RGX_BEGIN));
				else if (ch == '$') output.add(rgxss(RGX_END));
				else output.add(rgxss(RGX_SYMBOL,ch));

				canGreedy = false;
			}

			if (inBrace) return RGX_BRACE_MISMATCH;
			if (inOr) return RGX_BRACKET_MISMATCH;
			if (paren) return RGX_PARENTH_MISMATCH;

			if (escaped) output.add(rgxss(RGX_SYMBOL, '\\'));

			return RGX_NO_ERROR;
		}
	}
}
