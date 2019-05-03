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
			int startAnd = 0;
			int paren = 0;
			int amount = 0;
			bool canGreedy = false;

			for (auto& ch : pattern)
			{
				if (inBrace)
				{
					if (ch == '}')
					{
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
					}
					continue;
				}

				switch (startAnd)
				{
					case 1:
						if (ch != '?') break;
						output.add(rgxss(RGX_QUERY));
						startAnd = 2;
						continue;
					case 2:
						if (ch == '<')
						{
							output.add(rgxss(RGX_PREVIOUS));
							startAnd = 3;
							continue;
						}
						else if (ch == '-')
						{
							output.add(rgxss(RGX_DASH));
							startAnd = 0;
							continue;
						}
					case 3:
						if (ch == '!')
						{
							output.add(rgxss(RGX_BANG));
							continue;
						}
						else if (ch == '=')
						{
							output.add(rgxss(RGX_EQUALS));
							continue;
						}
						startAnd = 0;
				}

				if (escaped)
				{
					output.add(rgxsesc(ch));
					continue;
				}

				if (ch == '\\')
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
						else if ((ch == '-') && orForward)
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
					++paren;
					startAnd = 1;
					output.add(rgxss(RGX_LPAREN));
					continue;
				}

				if (ch == '{')
				{
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
