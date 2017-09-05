#pragma once
#ifndef REMAINDER_H_INCLUDED
#define REMAINDER_H_INCLUDED

#include <complex>

namespace z
{
    namespace math
    {
        /**
         * \brief Calculate the remainder of integer division \b x/y.
         *
         * \param x the numerator.
         * \param y the denominator.
         *
         * \return The remainder of integer \b x/y.
         *
         * \see remainder(const std::complex<long>&,const std::complex<long>&)
         */
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

        /**
         * \brief Calculate the remainder of complex division \b x/y.
         *
         * Performs integer division on numbers \b x and \b y of
         * \b std::complex type to calculate a remainder.
         *
         * \param x the numerator.
         * \param y the denominator.
         *
         * \return The remainder of complex integer division \b x/y.
         *
         * \see remainder(long,long)
         */
        const std::complex<long> remainder(const std::complex<long>& x,
                                           const std::complex<long>& y)
        {
            if ((y.real() == 0) && (y.imag() == 0))
                return std::complex<long>();

            long _norm = std::norm(y);

            long _imag = (x.imag()*y.real() - x.real()*y.imag()) / _norm;
            long _real = (x.real()*y.real() + x.imag()*y.imag()) / _norm;


            std::complex<long> _remd = x - (y * std::complex<long>(_real, _imag));


            return _remd;
        }
    }
}

#endif // REMAINDER_H_INCLUDED
