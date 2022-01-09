#include "fibonacci.hpp"
#include "constants.hpp"
#include <math.h>

namespace z
{
	namespace math
	{
		long long fibonacci(int n) noexcept
		{
			double PHI_n  = pow(phi, n);
			double Nphi_n = pow(-phi + 1, n);

			double fib_n  = (PHI_n - Nphi_n) / sqrt5;

			return (long long)fib_n;
		}
	}
}
