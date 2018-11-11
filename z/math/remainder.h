#pragma once

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
		long remainder(long x, long y);

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
		std::complex<long> remainder(const std::complex<long>& x, const std::complex<long>& y);
	}
}
