/**
 * File:            factorial.h
 * Namespace:       z::math
 *
 * Description:     A simple function that returns the
 *                  factorial of the given input as an
 *                  (unsigned long long).
 *                  Note that this function is only
 *                  accurate to factorial(64).
 *
 *                  This function will return 0 if the
 *                  given input is negative or out of
 *                  the accurate range.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   19 Jul. 2017
**/

#pragma once
#ifndef FACTORIAL_H_INCLUDED
#define FACTORIAL_H_INCLUDED

#include <cmath>

#define FACTORIAL_MAX_INPUT_INT 21
#define FACTORIAL_MAX_INPUT_DBL 21.439

namespace z
{
    namespace math
    {
        long factorial(long n)
        {
            if ((n < 0) || (n > FACTORIAL_MAX_INPUT_INT))
                return -1;


            long result = 1;

            for (long i=2; i<=n; i++)
            {
                result *= i;
            }

            return result;
        }



        double factorial(double x)
        {
            if ((x < 0) || (x > FACTORIAL_MAX_INPUT_DBL))
                return -1;

            if (x == (long)x)
                return (double)factorial((long)x);
            else
                return std::tgamma(x);
        }
    }
}

#endif // FACTORIAL_H_INCLUDED
