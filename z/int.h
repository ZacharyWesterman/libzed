/**
 * \file z/int.h
 *
 * Contains default integer type.
**/

#pragma once
#ifndef INT_H_INCLUDED
#define INT_H_INCLUDED

namespace z
{
    /**
     * \brief Default integer type used by other libraries.
     *
     * For some code, very large integers may not be required.
     * By default, all related libraries use \b long (usually 64-bit)
     * integers. However, if the compiler flag \b Z_USE_SHORT_INT is set,
     * then shorter (usu. 32-bit) integers are used. If he compiler flag
     * \b Z_USE_LONG_INT is set, then longer (64 to 128 bit) integers are used.
     */
    #ifdef Z_USE_SHORT_INT
    typedef int Int;
    #elif Z_USE_LONG_INT
    typedef long long Int;
    #else
    typedef long Int;
    #endif
}

#endif // INT_H_INCLUDED
