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
 * Last modified:   3 Feb. 2017
**/

#pragma once
#ifndef FACTORIAL_H_INCLUDED
#define FACTORIAL_H_INCLUDED

namespace z
{
    namespace math
    {
        unsigned long long factorial(int n)
        {
            if ((n < 0) || (n > 64))
                return 0;


            unsigned long long result = 1;

            for (int i=2; i<=n; i++)
            {
                result *= (unsigned long long)i;
            }

            return result;
        }
    }
}

#endif // FACTORIAL_H_INCLUDED
