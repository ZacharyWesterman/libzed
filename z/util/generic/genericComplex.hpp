#pragma once

#include "generic.hpp"

namespace z
{
	namespace util
	{
		/**
		 * \brief An implementation of the generic type for complex numbers.
		 */
		class genericComplex : public generic
		{
		private:
			std::complex<double> value;

		public:
			/**
			 * \brief Default constructor.
			 *
			 * \param init The complex number to initialize as.
			 */
			genericComplex(const std::complex<double>& init);

			bool boolean() const;

			long long integer() const;
			double floating() const;
			std::complex<double> complex() const;
			zstring string() const;

			bool isInteger() const;
			bool isFloating() const;
			bool isComplex() const;

			bool isArithmetic() const;

			datatype type() const;
			zstring typeString() const;

			generic* duplicate() const;
		};
	}
}
