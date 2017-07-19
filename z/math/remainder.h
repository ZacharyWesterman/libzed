/**
 * File:            remainder.h
 * Namespace:       z::math
 *
 * Description:     Functions to return the remainder of the
 *                  division of two numbers. I have added
 *                  remainder functions for both real and
 *                  complex numbers.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   19 Jul. 2017
**/

#pragma once
#ifndef REMAINDER_H_INCLUDED
#define REMAINDER_H_INCLUDED

#include <complex>

namespace z
{
    namespace math
    {
            const long remainder(long x, long y)
            {
                if (y == 0)
                    return 0;

                if (x < 0)
                    x = -x;
                if (y < 0)
                    y = -y;


                while (x >= y)
                {
                    x -= y;
                }

                return x;
            }



            const std::complex<long> remainder(std::complex<long> x, std::complex<long> y)
            {
                if ((y.real() == 0) && (y.imag() == 0))
                    return std::complex<long>();


                long norm_x, norm_y;

                norm_x = (x.real()*x.real()) + (x.imag()*x.imag());
                norm_y = (y.real()*y.real()) + (y.imag()*y.imag());


                if (x.real() < 0)
                    x = std::complex<long>(-x.real(), x.imag());
                if (x.imag() < 0)
                    x = std::complex<long>(x.real(), -x.imag());
                if (y.real() < 0)
                    y = std::complex<long>(-y.real(), y.imag());
                if (y.imag() < 0)
                    y = std::complex<long>(y.real(), -y.imag());


                while (norm_x >= norm_y)
                {
                    x -= y;

                    norm_x = (x.real()*x.real()) + (x.imag()*x.imag());
                    norm_y = (y.real()*y.real()) + (y.imag()*y.imag());
                }

                return x;
            }
    }
}

#endif // REMAINDER_H_INCLUDED
