#include "charFunctions.h"

namespace z
{
	namespace core
	{
		bool isUpperAlpha(uint32_t ch)
		{
			return ((ch >= 'A') &&  //from A
					(ch <= 'Z'));	//to Z
		}

		bool isLowerAlpha(uint32_t ch)
		{
			return ((ch >= 'a') &&  //from a
					(ch <= 'z'));	//to z
		}

		uint32_t toUpper(uint32_t ch)
		{
			if (isLowerAlpha(ch)) ch = ch - 'a' + 'A';

			return ch;
		}

		uint32_t toLower(uint32_t ch)
		{
			if (isUpperAlpha(ch)) ch = ch - 'A' + 'a';

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
	}
}
