#pragma once

#include "generic.h"

namespace z
{
	namespace util
	{
		class genericPointer : public generic
		{
		private:
			void* reference;

		public:
			genericPointer(void* init);

			bool boolean() const;

			void* pointer() const;
			long long integer() const;
			double floating() const;
			std::complex<double> complex() const;
			core::string<ZED_GENERIC_ENCODING> string() const;

			bool isPointer() const;

			bool isNull() const;

			datatype type() const;
			core::string<ZED_GENERIC_ENCODING> typeString() const;

			generic* duplicate() const;
		};
	}
}
