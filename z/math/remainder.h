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
 * Last modified:   2 Aug. 2017
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


            #ifdef _GLIBCXX_COMPLEX

            const std::complex<long> remainder(std::complex<long> x, std::complex<long> y)
            {
                if ((y.real() == 0) && (y.imag() == 0))
                    return std::complex<long>();

                long _norm = std::norm(y);

                long _imag = (x.imag()*y.real() - x.real()*y.imag()) / _norm;
                long _real = (x.real()*y.real() + x.imag()*y.imag()) / _norm;


                std::complex<long> _remd = x - (y * std::complex<long>(_real, _imag));


                return _remd;
            }

            #endif // _GLIBCXX_COMPLEX
    }
}

#endif // REMAINDER_H_INCLUDED
