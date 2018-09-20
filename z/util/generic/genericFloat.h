#pragma once

#include "generic.h"

namespace z
{
	namespace util
	{
		/**
		 * \brief An implementation of the generic type for floating-point numbers.
		 */
		class genericFloat : public generic
		{
		private:
			double value;

		public:
			/**
			 * \brief Default constructor.
			 *
			 * \param init The floating-point number to initialize as.
			 */
			genericFloat(double init);

			bool boolean() const;

			long long integer() const;
			double floating() const;
			std::complex<double> complex() const;
			core::string<ZED_GENERIC_ENCODING> string() const;

			bool isInteger() const;
			bool isFloating() const;

			bool isArithmetic() const;

			datatype type() const;
			core::string<ZED_GENERIC_ENCODING> typeString() const;

			generic* duplicate() const;
		};
	}
}
