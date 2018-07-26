#pragma once

template <encoding E>
uint8_t rgxscan(const core::string<E>& pattern, core::array<rgxss>& output)
{
	bool inOr = false;
	bool startOr = false;
	bool inBrace = false;
	int paren = 0;

	uint8_t last = 0;

	for (size_t i=0; i<pattern.length(); i++)
	{
		uint32_t ch = pattern[i];
		uint32_t prev = pattern[i-1];
		uint32_t next = pattern[i+1];

		bool braceValid = false;

		switch (ch)
		{
			case '(':
				if (inOr)
				{
					output.add(rgxss(last=RGX_SYMBOL, ch));
				}
				else
				{
					output.add(rgxss(last=RGX_LPAREN));
					paren++;
				}
				break;
			case ')':
				if (inOr)
				{
					output.add(rgxss(last=RGX_SYMBOL, ch));
				}
				else
				{
					output.add(rgxss(last=RGX_RPAREN));
					paren--;
				}
				break;
			case '[':
				if (inOr)
				{
					output.add(rgxss(last=RGX_SYMBOL, ch));
				}
				else
				{
					inOr = startOr = true;
					output.add(rgxss(last=RGX_LBRACKET));
				}
				break;
			case ']':
				if (inOr)
				{
					if (startOr)
						output.add(rgxss(last=RGX_SYMBOL, ch));
					else
						output.add(rgxss(last=RGX_LBRACKET));
				}
				else return RGX_BRACKET_MISMATCH;
				break;
			case '-':
				if (inOr)
				{
					if (startOr || (next == ']'))
						output.add(rgxss(last=RGX_SYMBOL, ch));
					else
						output.add(rgxss(last=RGX_DASH));
				}
				else
				{
					if ((last == RGX_LPAREN) || (last == RGX_RPAREN) || (next == '(') || (next == '['))
						output.add(rgxss(last=RGX_SYMBOL, ch));
					else
						output.add(rgxss(last=RGX_DASH))
				}
				break;
			case '?':
				if (inOr || (last == RGX_LPAREN))
					output.add(rgxss(last=RGX_SYMBOL, ch));
				else
					output.add(rgxss(last=RGX_QUESTION));
				break;
			case '|':
				if (inOr)
					output.add(rgxss(last=RGX_SYMBOL, ch));
				else
					output.add(rgxss(last=RGX_COLUMN))
				break;
			case '\\':
				rgxss esc = rgxsesc(next);
				last = esc.id();
				output.add(esc);
				i++;
				break;
			case '*':
				if (inOr || (last == RGX_LPAREN))
					output.add(rgxss(last=RGX_SYMBOL, ch));
				else
					output.add(rgxss(last=RGX_ASTERISK));
				break;
			case ',':
				braceValid = true;
				if (inBrace)
					output.add(rgxss(last=RGX_COMMA));
				else
					output.add(rgxss(last=RGX_SYMBOL, ch));
				break;
			case '{':
				if (inOr || (last == RGX_LPAREN))
				{
					output.add(rgxss(last=RGX_SYMBOL, ch));
				}
				else
				{
					inBrace = braceValid = true;
					output.add(rgxss(last=RGX_LBRACE));
				}
				break;
			case '}':
				if (inBrace)
				{
					inBrace = false;
					output.add(rgxss(last=RGX_RBRACE));
				}
				else
				{
					output.add(rgxss(last=RGX_SYMBOL, ch));
				}
				break;
			case '^':
				if (startOr)
					output.add(rgxss(last=RGX_NOT));
				else
					output.add(rgxss(last=RGX_BEGIN));
				break;
			case '$':
				output.add(rgxss(last=RGX_END));
				break;
			default:
				braceValid = ((ch >= '0') && (ch <= '9'));
				output.add(rgxss(last=RGX_SYMBOL, ch));
		}

		if (inBrace && !braceValid) return RGX_BRACE_INVALID;
		if (startOr && (ch != '[')) startOr = false;
	}

	if (inBrace) return RGX_BRACE_MISMATCH;
	if (inOr) return RGX_BRACKET_MISMATCH;
	if (paren) return RGX_PARENTH_MISMATCH;

	return RGX_NO_ERROR;
}
