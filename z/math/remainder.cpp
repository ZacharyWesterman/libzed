#include "remainder.hpp"

namespace z
{
	namespace math
	{
		long remainder(long x, long y)
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

		std::complex<long> remainder(const std::complex<long>& x, const std::complex<long>& y)
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
