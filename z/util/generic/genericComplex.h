#pragma once

#include "generic.h"

namespace z
{
	namespace util
	{
		class genericComplex : public generic
		{
		private:
			std::complex<double> value;

		public:
			genericComplex(const std::complex<double>& init);

			bool boolean() const;

			long long integer() const;
			double floating() const;
			std::complex<double> complex() const;
			core::string<ZED_GENERIC_ENCODING> string() const;

			bool isInteger() const;
			bool isFloating() const;
			bool isComplex() const;

			bool isArithmetic() const;

			datatype type() const;
			core::string<ZED_GENERIC_ENCODING> typeString() const;

			generic* duplicate() const;
		};
	}
}
