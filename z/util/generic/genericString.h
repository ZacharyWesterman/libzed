#pragma once

#include "generic.h"

namespace z
{
	namespace util
	{
		/**
		 * \brief An implementation of the generic type for strings.
		 */
		class genericString : public generic
		{
		private:
			core::string<ZED_GENERIC_ENCODING> data;

		public:
			/**
			 * \brief String constructor.
			 *
			 * \param init The string to initialize as.
			 */
			genericString(const core::string<ZED_GENERIC_ENCODING>& init);

			/**
			 * \brief Empty constructor.
			 *
			 * Initializes this object as an empty string.
			 */
			genericString();

			bool boolean() const;

			long long integer() const;
			double floating() const;
			std::complex<double> complex() const;
			core::string<ZED_GENERIC_ENCODING> string() const;

			bool isInteger() const;
			bool isFloating() const;
			bool isComplex() const;
			bool isString() const;

			bool isArithmetic() const;
			bool isNull() const;

			size_t length() const;

			int find(generic* element) const;
			bool add(generic* element);
			bool insert(generic* element, size_t index);
			bool remove(size_t index, int count);

			datatype type() const;
			core::string<ZED_GENERIC_ENCODING> typeString() const;

			generic* duplicate() const;
		};
	}
}