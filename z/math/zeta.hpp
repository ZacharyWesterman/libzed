#pragma once

namespace z
{
	namespace math
	{
		/**
		* \brief A function for approximating Reimann-zeta sums
		*
		* Computes zeta(s) with reasonable precision.
		*
		* \param s An integer greater than 1
		*
		* \return An approximation of zeta(s)
		*
		* \threadsafe_function_yes
		*/
		double zeta(const int s) noexcept;
	}
}
