#pragma once

#include "generic.h"

namespace z
{
	namespace util
	{
		class genericNull : public generic
		{
		public:
			bool boolean() const;

			long long integer() const;
			double floating() const;
			std::complex<double> complex() const;
			core::string<ZED_GENERIC_ENCODING> string() const;

			datatype type() const;
			core::string<ZED_GENERIC_ENCODING> typeString() const;

			bool isNull() const;

			generic* duplicate() const;
		};
	}
}
