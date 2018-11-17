#pragma once

#include "generic.h"

namespace z
{
	namespace util
	{
		/**
		 * \brief An implementation of the generic type for null data.
		 */
		class genericNull : public generic
		{
		public:
			bool boolean() const;

			long long integer() const;
			double floating() const;
			std::complex<double> complex() const;
			core::string<> string() const;

			datatype type() const;
			core::string<> typeString() const;

			bool isNull() const;

			generic* duplicate() const;
		};
	}
}
