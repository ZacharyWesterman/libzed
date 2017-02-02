#pragma once
#ifndef PAD_STRING_H_INCLUDED
#define PAD_STRING_H_INCLUDED

#include "../string.h"

namespace core
{
    //templates for pad function
    //pad function creates a string containing n of the given character
    template <typename CHAR>
    string<CHAR> padChar(CHAR character, int n)
    {
        string<CHAR> str;

        for (int i=0; i<n; i++)
            str += character;

        return str;
    }
}

#endif // PAD_STRING_H_INCLUDED
