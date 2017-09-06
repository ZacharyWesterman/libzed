/**
 * \file z/float.h
 *
 * Contains default floating-point type and a
 * struct for manipulating floating-point values.
**/

#pragma once
#ifndef FLOAT_H_INCLUDED
#define FLOAT_H_INCLUDED

namespace z
{
    /**
     * \brief Default floating-point type used by other libraries.
     *
     * For some code, very high precision with numbers may be required.
     * By default, all related libraries use single-precision floats.
     * However, if the compiler flag \b Z_USE_DOUBLE is set, then
     * double-precision floats are used instead.
     */
    #ifdef Z_USE_DOUBLE
    typedef double Float;
    #else
    typedef float Float;
    #endif // Z_USE_DOUBLE

    /**
     * \brief A union for easily manipulating
     * floating-point numbers.
     */
    typedef union
    {
        Float value; ///< The floating-point value.


        struct
        {
            #ifdef Z_USE_DOUBLE
            unsigned long mantissa : 52; ///< Fractional part.
            unsigned int exponent : 11; ///< Biased exponent.
            #else
            unsigned int mantissa : 23; ///< Fractional part.
            unsigned int exponent : 8; ///< Biased exponent.
            #endif // Z_USE_DOUBLE
            unsigned long sign : 1; ///< Sign.
        };

        /**
         * \brief Give the actual exponent.
         */
        inline int expUnBias()
        {
            #ifdef Z_USE_DOUBLE
            return (int)exponent - 1023;
            #else
            return (int)exponent - 127;
            #endif // Z_USE_DOUBLE
        }

    } Float_cast;

}

#endif // FLOAT_H_INCLUDED
