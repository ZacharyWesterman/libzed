#pragma once

#include "generic.h"
#include <z/core/refArray.h>

namespace z
{
	namespace util
	{
		class genericArray : public generic
		{
		private:
			core::refArray<generic*> data;

		public:
			genericArray(const core::array<generic*>& init);
			~genericArray();

			bool boolean() const;

			long long integer() const;
			double floating() const;
			std::complex<double> complex() const;
			core::string<ZED_GENERIC_ENCODING> string() const;
			core::array<generic*> array() const;

			bool isArray() const;

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
