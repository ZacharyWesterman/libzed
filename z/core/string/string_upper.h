/**
 * File:            string_upper.h
 * Namespace:       z::core
 *
 * Description:     A template function to convert strings
 *                  to uppercase.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   2 Feb. 2017
**/

#pragma once
#ifndef STRING_UPPER_H_INCLUDED
#define STRING_UPPER_H_INCLUDED

#include "string.h"

namespace z
{
    namespace core
    {
        ///upper() template for strings
        //template function to return the lowercase of the char string
        template <typename CHAR>
        string<CHAR> upper(const string<CHAR>& input)
        {
            int len = input.length()+1;

            CHAR buffer[len];

            for (int i=0; i<len; i++)
                if ((input[i] >= (CHAR)97) && (input[i] <= (CHAR)122))
                    buffer[i] = input[i] - (CHAR)97 + (CHAR)65;
                else
                    buffer[i] = input[i];

            return string<CHAR>(buffer);
        }
    }
}

#endif // STRING_UPPER_H_INCLUDED
