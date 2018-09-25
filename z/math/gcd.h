#pragma once

#include <complex>

namespace z
{
	namespace math
	{
		/**
		 * \brief Calculate the greatest common denominator of
		 * two integers.
		 *
		 * \param x the first integer.
		 * \param y the second integer.
		 *
		 * \return The greatest common denominator of both numbers.
		 *
		 * \see gcd(const std::complex<long>&,const std::complex<long>&)
		 */
		long gcd(long x, long y);

		/**
		 * \brief Calculate the greatest common denominator of
		 * two complex integers.
		 *
		 * \param x the first integer of type \b std::complex.
		 * \param y the second integer of type \b std::complex.
		 *
		 * \return The greatest common denominator of both numbers.
		 *
		 * \see gcd(long,long)
		 */
		std::complex<long> gcd(const std::complex<long>& x, const std::complex<long>& y);
	}
}
