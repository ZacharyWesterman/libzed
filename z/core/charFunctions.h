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
		 * \return \b True if the character is uppercase.
		 * \b False otherwise.
         */
        template <typename CHAR>
        bool isUpperAlpha(const CHAR ch)
        {
            return ((ch >= (CHAR)65) &&  //from A
                    (ch <= (CHAR)90));    //to Z
        }

        /**
         * \brief Check if the given character is a lowercase
         * alphabetic character.
		 *
		 * \param ch the character to check.
		 *
		 * \return \b True if the character is lowercase.
		 * \b False otherwise.
         */
        template <typename CHAR>
        bool isLowerAlpha(const CHAR ch)
        {
            return ((ch >= (CHAR)97) &&  //from a
                    (ch <= (CHAR)122));    //to z
        }

		/**
		 * \brief Convert the given character to uppercase.
		 *
		 * \param ch the character to convert.
		 *
		 * \return The character, converted to uppercase if
		 * applicable.
		 */
        template <typename CHAR>
        CHAR toUpper(CHAR ch)
        {
            if (isLowerAlpha(ch)) ch = ch - (CHAR)97 + (CHAR)65;

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
        template <typename CHAR>
        CHAR toLower(CHAR ch)
        {
            if (isUpperAlpha(ch)) ch = ch - (CHAR)65 + (CHAR)97;

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
        template <typename CHAR>
        bool isAlpha(const CHAR ch)
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
		 * If it is not a numeric character, returns \b -1.
         */
        template <typename CHAR>
        Int numeralValue(const CHAR ch)
        {
            if ((ch >= (CHAR)48) &&  //from 0
                (ch <= (CHAR)57))    //to 9
                return (ch - 48);
            else if (isLowerAlpha(ch))    //a-z
                return (ch - 87);
            else if (isUpperAlpha(ch))    //A-Z
                return (ch - 55);
            else
                return -1;
        }

		inline Int numeral(Int value)
		{
			if ((value > 36) || (value < 1))
				return '0';

			if (value < 10)
				return value + '0';
			else
				return (value - 10) + 'A';
		}

        /**
         * \brief Check if the given character is numeric under
         * the given base.
         */
        template <typename CHAR>
        inline bool isNumeric(const CHAR ch, Int base = 10)
        {
            Int value = numeralValue(ch);
            return ((value < base) && (value > -1));
        }

        /**
         * \brief Check if the given character is alphanumeric.
         */
        template <typename CHAR>
        inline bool isAlphaNumeric(const CHAR ch)
        {
            return (isAlpha(ch) ||  //is a letter
                    isNumeric(ch)); //or a number
        }

        /**
         * \brief Check if the given character is white space.
         */
        template <typename CHAR>
        bool isWhiteSpace(CHAR ch)
        {
            if ((ch == (CHAR)9)  || //tab
                (ch == (CHAR)10) || //newline
                (ch == (CHAR)13) || //carriage return
                (ch == (CHAR)32) || //space
                (ch == (CHAR)12) || //form feed
                (ch == (CHAR)11) || //vertical tab
                (ch == (CHAR)0))    //null
                return true;
            else
                return false;
        }

		//char c[4]
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


		//read UTF8 (with error compensation)
		uint32_t fromUTF8(uint8_t* c)
		{
			if (c)
			{
				uint32_t result = 0;
				bool read = true;

				int i = 0;

				while (read)
				{
					uint32_t chr = c[i];

					if (chr && (i < 4))
					{
						if (chr < 0x80)//0xxx xxxx
						{
							result = chr;
							read = false;
						}
						else if (chr < 0xC0)//10xx xxxx
						{
							result = (result << 6) + (chr & 0x3F);
							i++;
						}
						else if (chr < 0xE0)//110x xxxx, 10xx xxxx
						{
							result = (chr & 0x1F);
							i+=3; //1 more char after this
						}
						else if (chr < 0xF0)//1110 xxxx, 10xx xxxx, 10xx xxxx
						{
							result = (chr & 0x0F);
							i+=2;//2 more chars after this
						}
						else//1111 xxxx, 10xx xxxx, 10xx xxxx, 10xx xxxx
						{
							result = (chr & 0x0F);
							i++;
						}
					}
					else read = false;
				}

				//only get here when finished reading UTF8
				return result;
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

#endif // CHARFUNCTIONS_H_INCLUDED
