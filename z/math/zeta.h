#pragma once

namespace z
{
	namespace math
	{
		/**
		 * \fn Float zeta(const int s)
		 * \param s: an integer greater than 1
		 * \return An approximation of zeta(s)
		 *
		 * \brief A function for approximating Reimann-zeta sums
		 *
		 * Computes zeta(s) with reasonable precision.
		 */
		double zeta(const int s);
	}
}
