/**
 * File:            string_remove_whiteSpace.h
 * Namespace:       z::core
 *
 * Description:     Template functions to remove bordering
 *                  white space from strings.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Feb. 2017
**/

#pragma once
#ifndef STRING_REMOVE_WHITESPACE_H_INCLUDED
#define STRING_REMOVE_WHITESPACE_H_INCLUDED

#include "../string.h"
#include "char_is_whiteSpace.h"

namespace z
{
    namespace core
    {
        ///Removes whitespace from the end (right side) of the string
        ///and returns the result.
        template <typename CHAR>
        string<CHAR> remove_whitespace_R(const string<CHAR>& input)
        {
            int pos = input.length() - 1;

            while ((pos >= 0) && is_white_space(input[pos]))
                pos--;

            string<CHAR> output = input.substr(0, pos);

            return output;
        }


        ///Removes whitespace from the beginning (left side) of the string
        ///and returns the result.
        template <typename CHAR>
        string<CHAR> remove_whitespace_L(const string<CHAR>& input)
        {
            int pos = 0;

            while ((pos < input.length()) && is_white_space(input[pos]))
                pos++;

            string<CHAR> output = &input.str()[pos];

            return output;
        }


        ///Removes whitespace from both sides of the string
        ///and returns the result.
        template <typename CHAR>
        string<CHAR> remove_whitespace(const string<CHAR>& input)
        {
            string<CHAR> output = remove_whitespace_L(input);

            return remove_whitespace_R(output);
        }
    }
}

#endif // STRING_REMOVE_WHITESPACE_H_INCLUDED
