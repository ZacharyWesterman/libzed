#include "stringIterator.hpp"
#include "charFunctions.hpp"

namespace z
{
	namespace core
	{
		stringIterator::stringIterator(uint8_t* ptr, size_t offset, encoding format) noexcept
		{
			switch(format)
			{
				case ascii:
					data = ptr + offset;
					chr = data[0];
					increment = [](stringIterator* iter){
						iter->data += 1;
						iter->chr = iter->data[0];
					};
					break;
				case utf8:
					data = ptr + offset;
					chr = fromUTF8(data);
					increment = [](stringIterator* iter){
						iter->data += lenFromUTF8(iter->data);
						iter->chr = fromUTF8(iter->data);
					};
					break;
				case utf16:
					data = ptr + (offset << 1);
					chr = ((uint16_t*)data)[0];
					increment = [](stringIterator* iter){
						iter->data += 2;
						auto tmp = (uint16_t*)iter->data;
						iter->chr = tmp[0];
					};
					break;
				case utf32:
					data = ptr + (offset << 2);
					chr = ((uint32_t*)data)[0];
					increment = [](stringIterator* iter){
						iter->data += 4;
						auto tmp = (uint32_t*)iter->data;
						iter->chr = tmp[0];
					};
					break;
			}
		}

		uint32_t stringIterator::operator*() const noexcept {
			return chr;
		}

		stringIterator stringIterator::operator++() noexcept
		{
			increment(this);
			return *this;
		}
	}
}
