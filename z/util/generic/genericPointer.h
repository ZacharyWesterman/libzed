#pragma once

#include "generic.h"

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
			core::string<> string() const;

			bool isPointer() const;

			bool isNull() const;

			datatype type() const;
			core::string<> typeString() const;

			generic* duplicate() const;
		};
	}
}
