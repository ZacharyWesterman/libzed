#pragma once

#include "generic.hpp"

namespace z
{
	namespace util
	{
		/**
		 * \brief An implementation of the generic type for nonspecific pointers.
		 */
		class genericPointer : public generic
		{
		private:
			void* reference;

		public:
			/**
			 * \brief Default constructor.
			 *
			 * \param init The pointer to initialize as.
			 */
			genericPointer(void* init);

			bool boolean() const;

			void* pointer() const;
			long long integer() const;
			double floating() const;
			std::complex<double> complex() const;
			zstring string() const;

			bool isPointer() const;

			bool isNull() const;

			datatype type() const;
			zstring typeString() const;

			generic* duplicate() const;
		};
	}
}
