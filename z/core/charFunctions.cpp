#include "charFunctions.h"

static const uint32_t begRangesGroup[] =
{
	L'A', L'a',
	L'À', L'à',
	L'Ø', L'ø',
	L'Έ', L'έ',
	L'Ό', L'ό',
	L'Α', L'α',
	L'Σ', L'σ',
	L'Ϙ', L'ϙ',
	L'Ϟ', L'ϟ',
	L'А', L'а',
	L'Ѐ', L'ѐ',
	0, 0
};

static const uint32_t endRangesGroup[] =
{
	L'Z', L'z',
	L'Ö', L'ö',
	L'Þ', L'þ',
	L'Ί', L'ί',
	L'Ώ', L'ώ',
	L'Ρ', L'ρ',
	L'Ϋ', L'ϋ',
	L'Ϝ', L'ϝ',
	L'Ϯ', L'ϯ',
	L'Я', L'я',
	L'Џ', L'џ',
	0, 0
};

static const uint32_t begRangesSequence[] =
{
	L'Ā', L'ā',
	L'Ĳ', L'ĳ',
	L'Ĺ', L'ĺ',
	L'Ŋ', L'ŋ',
	L'Ź', L'ź',
	L'Ơ', L'ơ',
	L'Ǎ', L'ǎ',
	L'Ǟ', L'ǟ',
	L'Ǹ', L'ǹ',
	L'Ȣ', L'ȣ',
	L'Ɇ', L'ɇ',
	L'Ḃ', L'ḃ',
	L'Ͱ', L'ͱ',
	L'Ѡ', L'ѡ',
	L'Ҋ', L'ҋ',
	L'Ӂ', L'ӂ',
	L'Ӑ', L'ӑ',
	0, 0
};

static const uint32_t endRangesSequence[] =
{
	L'Į', L'į',
	L'Ķ', L'ķ',
	L'Ň', L'ň',
	L'Ŷ', L'ŷ',
	L'Ž', L'ž',
	L'Ƥ', L'ƥ',
	L'Ǜ', L'ǜ',
	L'Ǯ', L'ǯ',
	L'Ȟ', L'ȟ',
	L'Ȳ', L'ȳ',
	L'Ɏ', L'ɏ',
	L'Ẅ', L'ẅ',
	L'Ͷ', L'ͷ',
	L'Ҁ', L'ҁ',
	L'Ҿ', L'ҿ',
	L'Ӎ', L'ӎ',
	L'Ӿ', L'ӿ',
	0, 0
};

static const uint32_t directCases[] =
{
	L'Ÿ', L'ÿ',
	L'Ƃ', L'ƃ',
	L'Ƅ', L'ƅ',
	L'Ƈ', L'ƈ',
	L'Ƌ', L'ƌ',
	L'Ƒ', L'ƒ',
	L'Ƙ', L'ƙ',
	L'Ƨ', L'ƨ',
	L'Ƭ', L'ƭ',
	L'Ư', L'ư',
	L'Ƴ', L'ƴ',
	L'Ƶ', L'ƶ',
	L'Ƹ', L'ƹ',
	L'Ƽ', L'ƽ',
	L'Ǵ', L'ǵ',
	L'Ȼ', L'ȼ',
	L'Ɂ', L'ɂ',
	L'Ỳ', L'ỳ',
	L'Ά', L'ά',
	L'Ϸ', L'ϸ',
	L'Ϻ', L'ϻ',
	0, 0
};

//These lowercase characters convert to uppercase, but not the other way
static const uint32_t alternateUpper[] =
{
	L'Σ', L'ς',
	0, 0
};

static const uint32_t camelConvs[] =
{
	L'Ǆ', L'ǅ', L'ǆ',
	L'Ǉ', L'ǈ', L'ǉ',
	L'Ǌ', L'ǋ', L'ǌ',
	L'Ǳ', L'ǲ', L'ǳ',
	0, 0, 0
};

namespace z
{
	namespace core
	{
		bool isUpperAlpha(uint32_t ch)
		{
			return ((ch >= 'A') && (ch <= 'Z'));
		}

		bool isLowerAlpha(uint32_t ch)
		{
			return ((ch >= 'a') &&  (ch <= 'z'));
		}

		bool isUpper(uint32_t ch)
		{
			return (toLower(ch) != ch);
		}

		bool isLower(uint32_t ch)
		{
			return (toUpper(ch) != ch);
		}

		uint32_t toUpper(uint32_t ch, bool camel)
		{
			uint32_t beg;

			int i=1;
			while((beg = begRangesGroup[i]))
			{
				if (ch < beg) break;

				if ((ch >= beg) && (ch <= endRangesGroup[i]))
					return (ch - beg + begRangesGroup[i-1]);

				i+=2;
			}

			i=1;
			while((beg = begRangesSequence[i]))
			{
				if (ch < beg) break;

				if ((ch >= beg) && (ch <= endRangesSequence[i]))
					return (ch - 1);

				i+=2;
			}

			i=1;
			while((beg = directCases[i]))
			{
				if (ch < beg) break;

				if (beg == ch)
					return directCases[i-1];

				i+=2;
			}

			i=1;
			while((beg = alternateUpper[i]))
			{
				if (ch < beg) break;

				if (beg == ch)
					return alternateUpper[i-1];

				i+=2;
			}

			i=0;
			while((beg = camelConvs[i]))
			{
				if (ch < beg) break;

				if (camel)
				{
					if ((ch == beg) || (ch == camelConvs[i+2]))
						return camelConvs[i+1];
				}
				else
				{
					if ((ch == camelConvs[i+1]) || (ch == camelConvs[i+2]))
						return beg;
				}

				i+=3;
			}

			return ch;
		}

		uint32_t toLower(uint32_t ch, bool alternate)
		{
			uint32_t beg;

			int i;

			if (alternate)
			{
				i=0;
				while((beg = alternateUpper[i]))
				{
					if (ch < beg) break;

					if (beg == ch)
						return alternateUpper[i+1];

					i+=2;
				}
			}

			i=0;
			while((beg = begRangesGroup[i]))
			{
				if (ch < beg) break;

				if ((ch >= beg) && (ch <= endRangesGroup[i]))
					return (ch - beg + begRangesGroup[i+1]);

				i+=2;
			}

			i=0;
			while((beg = begRangesSequence[i]))
			{
				if (ch < beg) break;

				if ((ch >= beg) && (ch <= endRangesSequence[i]))
					return (ch + 1);

				i+=2;
			}

			i=0;
			while((beg = directCases[i]))
			{
				if (ch < beg) break;

				if (beg == ch)
					return directCases[i+1];

				i+=2;
			}

			i=0;
			while((beg = camelConvs[i]))
			{
				if (ch < beg) break;

				if ((ch == beg) || (ch == camelConvs[i+1]))
					return camelConvs[i+2];

				i+=3;
			}

			return ch;
		}

		bool isAlpha(uint32_t ch)
		{
			return (isLowerAlpha(ch) || //is uppercase
					isUpperAlpha(ch));  //or lowercase
		}

		int numeralValue(uint32_t ch)
		{
			if ((ch >= '0') &&  //from 0
				(ch <= '9'))	//to 9
				return (ch - '0');
			else if (isLowerAlpha(ch))	//a-z
				return (ch - 'a' + 10);
			else if (isUpperAlpha(ch))	//A-Z
				return (ch - 'A' + 10);
			else
				return -1;
		}

		uint32_t numeral(int value)
		{
			if ((value > 36) || (value < 1))
				return '0';

			if (value < 10)
				return value + '0';
			else
				return (value - 10) + 'A';
		}

		bool isNumeric(uint32_t ch, int base)
		{
			int value = numeralValue(ch);
			return ((value < base) && (value > -1));
		}

		bool isAlphaNumeric(uint32_t ch)
		{
			return (isAlpha(ch) ||  //is a letter
					isNumeric(ch)); //or a number
		}

		bool isWhiteSpace(uint32_t ch)
		{
			if ((ch == 9)  || //tab
				(ch == 10) || //newline
				(ch == 13) || //carriage return
				(ch == 32) || //space
				(ch == 12) || //form feed
				(ch == 11) || //vertical tab
				(ch == 0))	//null
				return true;
			else
				return false;
		}

		int toUTF8(uint8_t* c, uint32_t chr)
		{
			if (chr < 0x80)
			{
				c[0] = chr;
				return 1;
			}
			else if (chr < 0x0800)
			{
				c[0] = ((chr >> 6) & 0x1F) + 0xC0;
				c[1] = (chr & 0x3F) + 0x80;
				return 2;
			}
			else if (chr < 0xFFFF)
			{
				c[0] = ((chr >> 12) & 0x0F) + 0xE0;
				c[1] = ((chr >> 6) & 0x3F) + 0x80;
				c[2] = (chr & 0x3F) + 0x80;
				return 3;
			}
			else
			{
				c[0] = ((chr >> 18) & 0x07) + 0xF0;
				c[1] = ((chr >> 12) & 0x3F) + 0x80;
				c[2] = ((chr >> 6) & 0x3F) + 0x80;
				c[3] = (chr & 0x3F) + 0x80;
				return 4;
			}
		}

		int lenToUTF8(uint32_t chr)
		{
			if (chr < 0x80)
				return 1;
			else if (chr < 0x0800)
				return 2;
			else if (chr < 0xFFFF)
				return 3;
			else
				return 4;
		}

		uint32_t fromUTF8(uint8_t* c)
		{
			if (c)
			{
				uint32_t chr = c[0];

				if (chr < 0x80)//0xxx xxxx
				{
					return chr;
				}
				else if (chr < 0xC0)//10xx xxxx
				{
					return '?'; //invalid UTF8
				}
				else if (chr < 0xE0)//110x xxxx, 10xx xxxx
				{
					if (c[1])
					{
						uint32_t res[] = {(chr & 0x1F) << 6,
										(uint32_t)(c[1] & 0x3F)};

						return res[0] + res[1];
					}
					else return '?';
				}
				else if (chr < 0xF0)//1110 xxxx, 10xx xxxx, 10xx xxxx
				{
					if (c[1] && c[2])
					{
						uint32_t res[] = {(chr & 0x0F) << 12,
										(uint32_t)(c[1] & 0x3F) << 6,
										(uint32_t)(c[2] & 0x3F)};

						return res[0] + res[1] + res[2];
					}
					else return '?';
				}
				else//1111 xxxx, 10xx xxxx, 10xx xxxx, 10xx xxxx
				{
					if (c[1] && c[2] && c[3])
					{
						uint32_t res[] = {(chr & 0x0F) << 18,
										(uint32_t)(c[1] & 0x3F) << 12,
										(uint32_t)(c[2] & 0x3F) << 6,
										(uint32_t)(c[3] & 0x3F)};

						return res[0] + res[1] + res[2] + res[3];
					}
					else return '?';
				}

			}

			return 0;
		}

		int lenFromUTF8(uint8_t* c)
		{
			if (c)
			{
				uint8_t chr = c[0];

				if (chr < 0x80)//0xxx xxxx
					return 1;
				else if (chr < 0xC0)//10xx xxxx
					return 0; //not a starting char in UTF8 format
				else if (chr < 0xE0)//110x xxxx, 10xx xxxx
					return 2;
				else if (chr < 0xF0)//1110 xxxx, 10xx xxxx, 10xx xxxx
					return 3;
				else//1111 xxxx, 10xx xxxx, 10xx xxxx, 10xx xxxx
					return 4;
			}
			else return 0;
		}

		bool isUTF8(uint8_t* c, size_t len)
		{
			if (c && len)
			{
				if (c[0] < 0x80)//0xxx xxxx
				{
					return true;
				}
				else if (c[0] < 0xC0)//10xx xxxx
				{
					return false; //invalid UTF8
				}
				else if (c[0] < 0xE0)//110x xxxx, 10xx xxxx
				{
					if (len < 2) return false;
					return ((c[1] & 0xC0) == 0xC0);
				}
				else if (c[0] < 0xF0)//1110 xxxx, 10xx xxxx, 10xx xxxx
				{
					if (len < 3) return false;
					return ((c[1] & 0xC0) == 0xC0) && ((c[2] & 0xC0) == 0xC0);
				}
				else//1111 xxxx, 10xx xxxx, 10xx xxxx, 10xx xxxx
				{
					if (len < 4) return false;
					return ((c[1] & 0xC0) == 0xC0) && ((c[2] & 0xC0) == 0xC0) && ((c[3] & 0xC0) == 0xC0);
				}
			}

			return false;
		}
	}
}
