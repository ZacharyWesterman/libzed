/**
 * File:            float.h
 * Namespace:       none
 * Description:     Contains struct for manipulating
 *                  floating-point values. By default,
 *                  uses single-precision floats. If
 *                  the compiler flag Z_USE_DOUBLE is
 *                  set, then double-precision is used
 *                  instead.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   24 Aug. 2017
**/

#pragma once
#ifndef FLOAT_H_INCLUDED
#define FLOAT_H_INCLUDED

namespace z
{

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
