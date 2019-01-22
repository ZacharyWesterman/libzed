#pragma once

#include <cstdint>
#include <cstddef>
#include <z/encoding.h>

namespace z
{
	namespace core
	{
		template <encoding E>
		class stringIterator
		{
		private:
			uint8_t* data;
			uint32_t chr;
		public:
			stringIterator(uint8_t* ptr, size_t offset);
			stringIterator operator++();
			bool operator!=(const stringIterator& other) const {return data != other.data;}
			const uint32_t& operator*() const {return chr;}
		};
	}
}
