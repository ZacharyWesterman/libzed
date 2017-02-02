#pragma once
#ifndef STRING_LOWER_H_INCLUDED
#define STRING_LOWER_H_INCLUDED

#include "../string.h"

namespace z
{
    namespace core
    {
        ///lower() functions for string<char> and string<wchar_t> ONLY
        //function to return the lowercase of the char string
        string<char> lower(const string<char>& input)
        {
            int len = input.length()+1;

            char buffer[len];

            for (int i=0; i<len; i++)
                if ((input[i] >= 'A') && (input[i] <= 'Z'))
                    buffer[i] = input[i] - 'A' + 'a';
                else
                    buffer[i] = input[i];

            return string<char>(buffer);
        }

        //function to return the lowercase of the wchar_t string
        string<wchar_t> lower(const string<wchar_t>& input)
        {
            int len = input.length()+1;

            wchar_t buffer[len];

            for (int i=0; i<len; i++)
                if ((input[i] >= L'A') && (input[i] <= L'Z'))
                    buffer[i] = input[i] - L'A' + L'a';
                else
                    buffer[i] = input[i];

            return string<wchar_t>(buffer);
        }
    }
}

#endif // STRING_LOWER_H_INCLUDED
