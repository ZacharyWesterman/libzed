/**
 * File:            dePad_string.h
 * Namespace:       z::core
 *
 * Description:     Template functions that remove all
 *                  consecutive instances of a given
 *                  character from the end(s) of a string.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   2 Feb. 2017
**/

#pragma once
#ifndef DEPAD_STRING_H_INCLUDED
#define DEPAD_STRING_H_INCLUDED

#include "string.h"

namespace z
{
    namespace core
    {
        ///templates for dePad functions
        //function to remove character padding from the left side
        template <typename CHAR>
        string<CHAR> dePadL(const string<CHAR>& input, CHAR pad_char)
        {
            int pos = 0;

            while ((pos < input.length()) && (input[pos] == pad_char))
                pos++;

            string<CHAR> output = &input.str()[pos];

            return output;
        }

        //function to remove character padding from the right side
        template <typename CHAR>
        string<CHAR> dePadR(const string<CHAR>& input, CHAR pad_char)
        {
            int pos = input.length() - 1;

            while ((pos >= 0) && (input[pos] == pad_char))
                pos--;


            string<CHAR> output = input.substr(0, pos);



            return output;
        }

        //function to remove character padding from both sides of a string
        template <typename CHAR>
        string<CHAR> dePad(const string<CHAR>& input, CHAR pad_char)
        {
            string<CHAR> output = dePadL(input, pad_char);

            return dePadR(output, pad_char);
        }
    }
}

#endif // DEPAD_STRING_H_INCLUDED
