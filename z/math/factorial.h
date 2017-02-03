/**
 * File:            factorial.h
 * Namespace:       z::math
 *
 * Description:     A simple function that returns the
 *                  factorial of the given input.
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
        long int fac(int n)
        {
            long int output = 1;

            for (long int i=2; i<=(long int)n; i++)
            {
                output *= i;
            }

            return output;
        }
    }
}

#endif // FACTORIAL_H_INCLUDED
