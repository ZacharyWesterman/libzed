#include "gcd.hpp"

namespace z {
namespace math {
long gcd(long x, long y) noexcept {
	if ((x == 0) && (y == 0)) {
		return 1;
	}

	long hi, lo;

	if (x > y) {
		hi = x;
		lo = y;
	} else {
		hi = y;
		lo = x;
	}

	if (hi < 0) {
		hi = -hi;
	}
	if (lo < 0) {
		lo = -lo;
	}

	do {
		while (hi >= lo) {
			hi -= lo;
		}

		long temp = hi;

		hi = lo;
		lo = temp;
	} while (lo);

	return hi;
}

std::complex<long> gcd(const std::complex<long> &x, const std::complex<long> &y) noexcept {
	long norm_x, norm_y;

	norm_x = (x.real() * x.real()) + (x.imag() * x.imag());
	norm_y = (y.real() * y.real()) + (y.imag() * y.imag());

	std::complex<long> hi, lo;
	long norm_hi, norm_lo;

	if (norm_x > norm_y) {
		hi = x;
		lo = y;
	} else {
		hi = y;
		lo = x;
	}

	if (hi.real() < 0) {
		hi = std::complex<long>(-hi.real(), hi.imag());
	}
	if (hi.imag() < 0) {
		hi = std::complex<long>(hi.real(), -hi.imag());
	}
	if (lo.real() < 0) {
		lo = std::complex<long>(-lo.real(), lo.imag());
	}
	if (lo.imag() < 0) {
		lo = std::complex<long>(lo.real(), -lo.imag());
	}

	norm_hi = (hi.real() * hi.real()) + (hi.imag() * hi.imag());
	norm_lo = (lo.real() * lo.real()) + (lo.imag() * lo.imag());
	do {
		while (norm_hi >= norm_lo) {
			hi -= lo;

			norm_hi = (hi.real() * hi.real()) + (hi.imag() * hi.imag());
			norm_lo = (lo.real() * lo.real()) + (lo.imag() * lo.imag());
		}

		std::complex<long> temp = hi;
		hi = lo;
		lo = temp;

		norm_hi = (hi.real() * hi.real()) + (hi.imag() * hi.imag());
		norm_lo = (lo.real() * lo.real()) + (lo.imag() * lo.imag());
	} while (norm_lo);

	return hi;
}
} // namespace math
} // namespace z
