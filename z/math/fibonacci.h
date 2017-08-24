/**
 * File:            fibonacci.h
 * Namespace:       z::math
 *
 * Description:     The fibonacci(n) function returns
 *                  the nth number in the Fibonacci
 *                  sequence. To avoid recursion and
 *                  for the sake of performance, this
 *                  function uses Binet's formula.
 *
 *                  Additionally, doubles are used to
 *                  allow for very large output values,
 *                  but anything up to fibonacci(46)
 *                  number safely be cast to a (long), up to
 *                  the fibonacci(92) can be cast to
 *                  (unsigned long) or a (long long). However,
 *                  the result is only accurate up to
 *                  fibonacci(71).
 *
 *                  Note: this function will return
 *                  nan (not a number) if the parameter
 *                  is anything other than a whole number.
 *                  It does, however, work with negatives.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Feb. 2017
**/

#pragma once
#ifndef FIBONACCI_H_INCLUDED
#define FIBONACCI_H_INCLUDED

#include "constants.h"
#include <math.h>

namespace z
{
    namespace math
    {
        zFloat fibonacci(zFloat n)
        {
            zFloat PHI_n  = pow(phi, n);
            zFloat Nphi_n = pow(-phi + 1, n);

            zFloat fib_n  = (PHI_n - Nphi_n) / sqrt5;

            return fib_n;
        }
    }
}

#endif // FIBONACCI_H_INCLUDED
