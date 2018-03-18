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

namespace z
{
    namespace core
    {
        /**
         * \brief Check if the given character is an uppercase
         * alphabetic character.
         */
        template <typename CHAR>
        bool isUpperAlpha(const CHAR character)
        {
            return ((character >= (CHAR)65) &&  //from A
                    (character <= (CHAR)90));    //to Z
        }

        /**
         * \brief Check if the given character is a lowercase
         * alphabetic character.
         */
        template <typename CHAR>
        bool isLowerAlpha(const CHAR character)
        {
            return ((character >= (CHAR)97) &&  //from a
                    (character <= (CHAR)122));    //to z
        }

        template <typename CHAR>
        CHAR toUpper(CHAR ch)
        {
            if (isLowerAlpha(ch)) ch = ch - (CHAR)97 + (CHAR)65;

            return ch;
        }

        template <typename CHAR>
        CHAR toLower(CHAR ch)
        {
            if (isUpperAlpha(ch)) ch = ch - (CHAR)65 + (CHAR)97;

            return ch;
        }

        /**
         * \brief Check if the given character is an
         * alphabetic character.
         */
        template <typename CHAR>
        bool isAlpha(const CHAR character)
        {
            return (isLowerAlpha(character) || //is uppercase
                    isUpperAlpha(character));  //or lowercase
        }

        /**
         * \brief Convert the given character to its
         * respective numeral value.
         */
        template <typename CHAR>
        int numeralValue(const CHAR character)
        {
            if ((character >= (CHAR)48) &&  //from 0
                (character <= (CHAR)57))    //to 9
                return (character - 48);
            else if ((character >= (CHAR)97) &&  //from a
                     (character <= (CHAR)122))    //to z
                return (character - 87);
            else if ((character >= (CHAR)65) &&  //from A
                     (character <= (CHAR)90))    //to Z
                return (character - 55);
            else
                return -1;
        }

        /**
         * \brief Check if the given character is numeric under
         * the given base.
         */
        template <typename CHAR>
        inline bool isNumeric(const CHAR character, int base = 10)
        {
            int value = numeralValue(character);
            return ((value < base) && (value > -1));
        }

        /**
         * \brief Check if the given character is alphanumeric.
         */
        template <typename CHAR>
        inline bool isAlphanumeric(const CHAR character)
        {
            return (isAlpha(character) ||  //is a letter
                    isNumeric(character)); //or a number
        }

        /**
         * \brief Check if the given character is white space.
         */
        template <typename CHAR>
        bool isWhiteSpace(CHAR character)
        {
            if ((character == (CHAR)9)  || //tab
                (character == (CHAR)10) || //newline
                (character == (CHAR)13) || //carriage return
                (character == (CHAR)32) || //space
                (character == (CHAR)12) || //form feed
                (character == (CHAR)11) || //vertical tab
                (character == (CHAR)0))    //null
                return true;
            else
                return false;
        }
    }
}

#endif // CHARFUNCTIONS_H_INCLUDED
