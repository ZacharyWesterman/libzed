#pragma once
#ifndef CHAR_IS_WHITESPACE_H_INCLUDED
#define CHAR_IS_WHITESPACE_H_INCLUDED

namespace core
{
    template <typename CHAR>
    bool is_white_space(CHAR character)
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

#endif // CHAR_IS_WHITESPACE_H_INCLUDED
