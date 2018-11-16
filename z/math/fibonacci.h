#pragma once

#include "constants.h"
#include <math.h>

namespace z
{
	namespace math
	{
		/**
		 * \brief Calculate the n<SUP>th</SUP>
		 * number in the Fibonacci sequence.
		 *
		 * To avoid recursion and
		 * for the sake of performance, this
		 * function uses Binet's formula.
		 * <BR>
		 * Additionally, doubles are used to
		 * allow for very large output values,
		 * but anything up to fibonacci(46)
		 * number safely be cast to a (long), up to
		 * the fibonacci(92) can be cast to
		 * (unsigned long) or a (long long). However,
		 * the result is only accurate up to
		 * fibonacci(71).
		 * <BR>
		 * Note: this function will return
		 * \b NaN if the parameter
		 * is anything other than a whole number.
		 * It does, however, work with negatives.
		 *
		 * \param n the index of the desired number in
		 * the Fibonacci sequence.
		 *
		 * \return The value of the n<SUP>th</SUP>
		 * number in the Fibonacci sequence.
		 *
		 * \threadsafe_function_yes
		 */
		double fibonacci(double n);
	}
}
