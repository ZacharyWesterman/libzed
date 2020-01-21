#pragma once

#include "generic.hpp"

namespace z
{
	namespace util
	{
		/**
		 * \brief An implementation of the generic type for integers.
		 */
		class genericInt : public generic
		{
		private:
			long long value;

		public:
			/**
			 * \brief Default constructor.
			 *
			 * \param init The integer to initialize as.
			 */
			genericInt(long long init);

			bool boolean() const;

			long long integer() const;
			double floating() const;
			std::complex<double> complex() const;
			zstring string() const;

			bool isInteger() const;

			bool isArithmetic() const;

			datatype type() const;
			zstring typeString() const;

			generic* duplicate() const;
		};
	}
}
