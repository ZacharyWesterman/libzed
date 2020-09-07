#pragma once

#include "generic.hpp"

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
			zstring data;

		public:
			/**
			 * \brief String constructor.
			 *
			 * \param init The string to initialize as.
			 */
			genericString(const zstring& init);

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
			zstring string() const;

			bool isInteger() const;
			bool isFloating() const;
			bool isComplex() const;
			bool isString() const;

			bool isArithmetic() const;
			bool isNull() const;

			int length() const;

			int find(generic* element) const;
			bool add(generic* element);
			bool insert(generic* element, int index);
			bool remove(int index, int count);

			datatype type() const;
			zstring typeString() const;

			generic* duplicate() const;
			size_t size() const noexcept;
		};
	}
}
