#include "rgxsesc.hpp"

namespace z
{
	namespace util
	{
		rgxss rgxsesc(uint32_t ch)
		{
			switch (ch)
			{
				case 's':
					return rgxss(RGX_SPACE);
				case 'S':
					return rgxss(RGX_NOT_SPACE);
				case 'a':
					return rgxss(RGX_ALPHA);
				case 'A':
					return rgxss(RGX_NOT_ALPHA);
				case 'd':
					return rgxss(RGX_DIGIT);
				case 'D':
					return rgxss(RGX_NOT_DIGIT);
				case 'l':
					return rgxss(RGX_ALNUM);
				case 'L':
					return rgxss(RGX_NOT_ALNUM);
				case 'w':
					return rgxss(RGX_WORD);
				case 'W':
					return rgxss(RGX_NOT_WORD);
				case 'n':
					return rgxss(RGX_NEWLINE);
				case 'b':
					return rgxss(RGX_BREAK);
				case 'B':
					return rgxss(RGX_NOT_BREAK);
				case 'p':
					return rgxss(RGX_PUNCT);
				case 'P':
					return rgxss(RGX_NOT_PUNCT);
				default:
					if (ch)
						return rgxss(RGX_SYMBOL, ch);
					else
						return rgxss(RGX_SYMBOL, '\\');
			}
		}
	}
}
