#include "zeta.hpp"

namespace z
{
	namespace math
	{
		double zeta(const int s) noexcept
		{
			double result = 1.0;

			double i = 2.0;

			//doubles can cause this computation to take too long,
			//so time out after 100,000 iterations.
			double oldResult = -1.0;
			while ((i<100000) && ((oldResult+0.0000001) < result))
			{
				double denomP = 1.0 / i;

				double partial = 1;
				for (int j=0; j<s; j++)
				{
					partial *= denomP;
				}

				result += partial;

				i++;
			}

			return result;
		}
	}
}
