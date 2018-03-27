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
    }
}

#endif // CHARFUNCTIONS_H_INCLUDED
