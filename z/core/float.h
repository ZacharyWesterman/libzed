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


#ifdef Z_USE_DOUBLE
    typedef double zFloat;

    /**
     * \brief A union for easily manipulating
     * floating-point numbers.
     */
    typedef union
    {
        double value; ///< The floating-point value.

        struct
        {
            unsigned long mantissa : 52; ///< Fractional part.
            unsigned long exponent : 11; ///< Biased exponent.
            unsigned long sign : 1; ///< Sign.
        };

        /**
         * \brief Give the actual exponent.
         */
        inline long expUnBias()
        {
            return (long)exponent - 1023;
        }
    } zFloat_cast;

#else
    typedef float zFloat;

    /**
     * \brief A union for easily manipulating
     * floating-point numbers.
     */
    typedef union
    {
        float value; ///< Floating-point value.

        struct
        {
            unsigned int mantissa : 23; ///< Fractional part.
            unsigned int exponent : 8; ///< Biased exponent.
            unsigned int sign : 1; ///< Sign.
        };

        /**
         * \brief Give the actual exponent.
         */
        inline long expUnBias()
        {
            return (int)exponent - 127;
        }
    } zFloat_cast;
#endif // Z_USE_DOUBLE



#endif // FLOAT_H_INCLUDED
