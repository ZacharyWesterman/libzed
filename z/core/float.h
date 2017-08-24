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

    typedef union
    {
        double value;

        struct
        {
            unsigned long mantissa : 52;
            unsigned long exponent : 11;
            unsigned long sign : 1;

            inline long expUnBias()
            {
                return (long)exponent - 1023;
            }
        } part;
    } zFloat_cast;

#else
    typedef float zFloat;

    typedef union
    {
        float value;

        struct
        {
            unsigned int mantissa : 23;
            unsigned int exponent : 8;
            unsigned int sign : 1;

            inline int expUnBias()
            {
                return (int)exponent - 127;
            }
        } part;
    } zFloat_cast;
#endif // Z_USE_DOUBLE



#endif // FLOAT_H_INCLUDED
