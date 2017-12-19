/**
 * \file z/char.h
 *
 * Contains default character type.
**/

#pragma once
#ifndef CHAR_H_INCLUDED
#define CHAR_H_INCLUDED

namespace z
{
    /**
     * \brief Default character type used by other libraries.
     *
     * For some code, wide characters may not be required.
     * By default, all related libraries use \b wchar_t
     * characters. However, if the compiler flag \b Z_USE_NARROW is set,
     * then \b char is used.
     */
    #ifdef Z_USE_NARROW
    typedef char Char;
    #else
    typedef wchar_t Char;
    #endif
}

#endif // CHAR_H_INCLUDED
