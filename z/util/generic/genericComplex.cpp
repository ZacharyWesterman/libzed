#include "genericComplex.h"

namespace z
{
	namespace util
	{
		genericComplex::genericComplex(const std::complex<double>& init) : value(init) {}

		bool genericComplex::boolean() const
		{
			return value.real() || value.imag();
		}

		long long genericComplex::integer() const
		{
			return value.real();
		}

		double genericComplex::floating() const
		{
			return value.real();
		}

		std::complex<double> genericComplex::complex() const
		{
			return value;
		}

		zstring genericComplex::string() const
		{
			return value;
		}

		bool genericComplex::isInteger() const
		{
			return !value.imag() && (((double)((long long)value.real())) == value.real());
		}

		bool genericComplex::isFloating() const
		{
			return !value.imag();
		}

		bool genericComplex::isComplex() const
		{
			return true;
		}

		bool genericComplex::isArithmetic() const
		{
			return true;
		}

		datatype genericComplex::type() const
		{
			return datatype::complex;
		}

		zstring genericComplex::typeString() const
		{
			return "COMPLEX";
		}

		generic* genericComplex::duplicate() const
		{
			return new genericComplex(value);
		}
	}
}
