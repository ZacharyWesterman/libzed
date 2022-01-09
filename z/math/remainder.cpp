#include "remainder.hpp"

std::complex<long> operator%(const std::complex<long>& x, const std::complex<long>& y) noexcept
{
	if ((y.real() == 0) && (y.imag() == 0))
		return std::complex<long>();

	long _norm = std::norm(y);

	long _imag = (x.imag()*y.real() - x.real()*y.imag()) / _norm;
	long _real = (x.real()*y.real() + x.imag()*y.imag()) / _norm;

	std::complex<long> _remd = x - (y * std::complex<long>(_real, _imag));

	return _remd;
}
