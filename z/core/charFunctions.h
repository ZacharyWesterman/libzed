/**
 * File:            charFunctions.h
 * Namespace:       z::core
 *
 * Description:     Template functions that return
 *                  if a character is alphanumeric.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   1 Sep. 2017
**/

#pragma once
#ifndef CHARFUNCTIONS_H_INCLUDED
#define CHARFUNCTIONS_H_INCLUDED

#include <z/int.h>

namespace z
{
    namespace core
    {
        /**
         * \brief Check if the given character is an uppercase
         * alphabetic character.
		 *
		 * \param ch the character to check.
		 *
		 * \return True if the character is uppercase. False otherwise.
         */
        bool isUpperAlpha(uint32_t ch)
        {
            return ((ch >= 'A') &&  //from A
                    (ch <= 'Z'));    //to Z
        }

        /**
         * \brief Check if the given character is a lowercase
         * alphabetic character.
		 *
		 * \param ch the character to check.
		 *
		 * \return True if the character is lowercase. False otherwise.
         */
        bool isLowerAlpha(uint32_t ch)
        {
            return ((ch >= 'a') &&  //from a
                    (ch <= 'z'));    //to z
        }

		/**
		 * \brief Convert the given character to uppercase.
		 *
		 * \param ch the character to convert.
		 *
		 * \return The character, converted to uppercase if
		 * applicable.
		 */
        uint32_t toUpper(uint32_t ch)
        {
            if (isLowerAlpha(ch)) ch = ch - 'a' + 'A';

            return ch;
        }

		/**
		 * \brief Convert the given character to lowercase.
		 *
		 * \param ch the character to convert.
		 *
		 * \return The character, converted to lowercase if
		 * applicable.
		 */
        uint32_t toLower(uint32_t ch)
        {
            if (isUpperAlpha(ch)) ch = ch - 'A' + 'a';

            return ch;
        }

        /**
         * \brief Check if the given character is an
         * alphabetic character.
		 *
		 * \param ch the character to check.
		 *
		 * \return \b True if the character is an alphabetic character.
		 * \b False otherwise.
         */
        bool isAlpha(uint32_t ch)
        {
            return (isLowerAlpha(ch) || //is uppercase
                    isUpperAlpha(ch));  //or lowercase
        }

        /**
         * \brief Convert the given character to its
         * respective numeral value.
		 *
		 * '0'-'9' give 0-9. from 'A'-'Z' give 10-36.
		 * Case insensitive.
		 *
		 * \param ch the character to check.
		 *
		 * \return The numeral value of the character.
		 * If it is not a numeric character, returns -1.
         */
        int numeralValue(uint32_t ch)
        {
            if ((ch >= '0') &&  //from 0
                (ch <= '9'))    //to 9
                return (ch - '0');
            else if (isLowerAlpha(ch))    //a-z
                return (ch - 'a' + 10);
            else if (isUpperAlpha(ch))    //A-Z
                return (ch - 'A' + 10);
            else
                return -1;
        }

		/**
         * \brief Convert the given value to its
         * respective numeral character.
		 *
		 * 0-9 give '0'-'9'. 10-36 give 'A'-'Z'.
		 *
		 * \param value the value to convert.
		 *
		 * \return The character numeral for the value.
		 * If it is not a numeral value, returns '0'.
         */
		inline uint32_t numeral(int value)
		{
			if ((value > 36) || (value < 1))
				return '0';

			if (value < 10)
				return value + '0';
			else
				return (value - 10) + 'A';
		}

        /**
         * \brief Check if the given character is numeric under the given base.
		 *
		 * \param ch The character to check.
		 * \param base The base for this character.
		 *
		 * \return True if the character is numeric under the given base. False otherwise.
         */
        inline bool isNumeric(uint32_t ch, int base = 10)
        {
            int value = numeralValue(ch);
            return ((value < base) && (value > -1));
        }

        /**
         * \brief Check if the given character is alphanumeric.
		 *
		 * \param ch The character to check.
		 *
		 * \return True if the character is alphanumeric. False otherwise.
         */
        inline bool isAlphaNumeric(uint32_t ch)
        {
            return (isAlpha(ch) ||  //is a letter
                    isNumeric(ch)); //or a number
        }

        /**
         * \brief Check if the given character is white space.
		 *
		 * \param ch The character to check.
		 *
		 * \return True if the character is a white space character. False otherwise.
         */
        bool isWhiteSpace(uint32_t ch)
        {
            if ((ch == 9)  || //tab
                (ch == 10) || //newline
                (ch == 13) || //carriage return
                (ch == 32) || //space
                (ch == 12) || //form feed
                (ch == 11) || //vertical tab
                (ch == 0))    //null
                return true;
            else
                return false;
        }

		/**
		 * \brief Convert a character to UTF-8 encoding.
		 *
		 * \param c The UTF-8 sequence, returned by pointer. Assumed to be a 4-item array.
		 * \param chr The character to convert to UTF-8.
		 *
		 * \return The number of characters in the UTF-8 sequence.
		 */
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

		/**
		 * \brief Get the length of the UTF-8 encoding for a character.
		 *
		 * \param chr The character to convert to UTF-8.
		 *
		 * \return The number of characters in the UTF-8 sequence.
		 */
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


		/**
		 * \brief Convert a character from UTF-8 to UTF32 encoding.
		 *
		 * \param c The UTF-8 sequence. Assumed to be a 4-item array.
		 *
		 * \return The character encoded as UTF32. '?' if input is not valid UTF-8.
		 */
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

		/**
		 * \brief Get the character length of a UTF-8 sequence.
		 *
		 * \param c The UTF-8 sequence. Assumed to be a 4-item array.
		 *
		 * \return The number of characters in the UTF-8 sequence. Assumed to be valid UTF-8.
		 */
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

#endif // CHARFUNCTIONS_H_INCLUDED
