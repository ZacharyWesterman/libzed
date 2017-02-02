#pragma once
#ifndef STRING_REMOVE_WHITESPACE_H_INCLUDED
#define STRING_REMOVE_WHITESPACE_H_INCLUDED

#include "../string.h"
#include "char_is_whiteSpace.h"

namespace core
{
    template <typename CHAR>
    string<CHAR> remove_whitespace_R(const string<CHAR>& input)
    {
        int pos = input.length() - 1;

        while ((pos >= 0) && is_white_space(input[pos]))
            pos--;

        string<CHAR> output = input.substr(0, pos);

        return output;
    }


    template <typename CHAR>
    string<CHAR> remove_whitespace_L(const string<CHAR>& input)
    {
        int pos = 0;

        while ((pos < input.length()) && is_white_space(input[pos]))
            pos++;

        string<CHAR> output = &input.str()[pos];

        return output;
    }


    template <typename CHAR>
    string<CHAR> remove_whitespace(const string<CHAR>& input)
    {
        string<CHAR> output = remove_whitespace_L(input);

        return remove_whitespace_R(output);
    }
}

#endif // STRING_REMOVE_WHITESPACE_H_INCLUDED
