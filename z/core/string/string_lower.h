/**
 * File:            string_lower.h
 * Namespace:       z::core
 *
 * Description:     A template function to convert strings
 *                  to lowercase.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   2 Feb. 2017
**/

#pragma once
#ifndef STRING_LOWER_H_INCLUDED
#define STRING_LOWER_H_INCLUDED

#include "string.h"

namespace z
{
    namespace core
    {
        ///lower() template for strings
        //template function to return the lowercase of the string
        template <typename CHAR>
        string<CHAR> lower(const string<CHAR>& input)
        {
            int len = input.length()+1;

            CHAR buffer[len];

            for (int i=0; i<len; i++)
                if ((input[i] >= (CHAR)65) && (input[i] <= (CHAR)90))
                    buffer[i] = input[i] - (CHAR)65 + (CHAR)97;
                else
                    buffer[i] = input[i];

            return string<CHAR>(buffer);
        }
    }
}

#endif // STRING_LOWER_H_INCLUDED
