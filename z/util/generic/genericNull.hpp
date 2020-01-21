#pragma once

#include "generic.hpp"

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
			zstring string() const;

			datatype type() const;
			zstring typeString() const;

			bool isNull() const;

			generic* duplicate() const;
		};
	}
}
