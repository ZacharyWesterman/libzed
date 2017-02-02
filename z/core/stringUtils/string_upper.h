#pragma once
#ifndef STRING_UPPER_H_INCLUDED
#define STRING_UPPER_H_INCLUDED

#include "../string.h"

namespace core
{
    ///upper() functions for string<char> and string<wchar_t> ONLY
    //function to return the lowercase of the char string
    string<char> upper(const string<char>& input)
    {
        int len = input.length()+1;

        char buffer[len];

        for (int i=0; i<len; i++)
            if ((input[i] >= 'a') && (input[i] <= 'z'))
                buffer[i] = input[i] - 'a' + 'A';
            else
                buffer[i] = input[i];

        return string<char>(buffer);
    }

    //function to return the lowercase of the wchar_t string
    string<wchar_t> upper(const string<wchar_t>& input)
    {
        int len = input.length()+1;

        wchar_t buffer[len];

        for (int i=0; i<len; i++)
            if ((input[i] >= L'a') && (input[i] <= L'z'))
                buffer[i] = input[i] - L'a' + L'A';
            else
                buffer[i] = input[i];

        return string<wchar_t>(buffer);
    }
}

#endif // STRING_UPPER_H_INCLUDED
