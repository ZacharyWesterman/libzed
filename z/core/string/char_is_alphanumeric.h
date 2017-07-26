/**
 * File:            char_is_alphanumeric.h
 * Namespace:       z::core
 *
 * Description:     Template functions that return
 *                  if a character is alphanumeric.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   26 Jul. 2017
**/

#pragma once
#ifndef CHAR_IS_ALPHANUMERIC_H_INCLUDED
#define CHAR_IS_ALPHANUMERIC_H_INCLUDED

namespace z
{
    namespace core
    {
        template <typename CHAR>
        bool is_upper_alpha(const CHAR character)
        {
            if ((character >= (CHAR)65) &&  //from A
                (character <= (CHAR)90))    //to Z
                return true;
            else
                return false;
        }


        template <typename CHAR>
        bool is_lower_alpha(const CHAR character)
        {
            if ((character >= (CHAR)97) &&  //from a
                (character <= (CHAR)122))    //to z
                return true;
            else
                return false;
        }


        template <typename CHAR>
        bool is_alpha(const CHAR character)
        {
            return (is_lower_alpha(character) || //is uppercase
                    is_upper_alpha(character));  //or lowercase
        }


        template <typename CHAR>
        int numeral_value(const CHAR character)
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

        template <typename CHAR>
        inline bool is_numeric(const CHAR character, int base = 10)
        {
            int value = numeral_value(character);
            return ((value < base) && (value > -1));
        }


        template <typename CHAR>
        bool is_alphanumeric(const CHAR character)
        {
            return (is_alpha(character) ||  //is a letter
                    is_numeric(character)); //or a number
        }
    }
}

#endif // CHAR_IS_ALPHANUMERIC_H_INCLUDED
