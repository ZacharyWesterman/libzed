#include "stringStream.hpp"
#include "charFunctions.hpp"

namespace z
{
	namespace core
	{
		void stringStream::put(uint8_t ch)
		{
			data.replace(streamIndex, 1, zstring((char)ch));
			++streamIndex;
		}

		void stringStream::put(uint8_t* str, int count, encoding format)
		{
			uint32_t* str32 = (uint32_t*)str;
			uint16_t* str16 = (uint16_t*)str;

			zstring res;
			res.increase(count);
			for (int i=0; i<count; ++i)
			{
				switch (format)
				{
				case utf32:
					res.append(str32[i]);
					break;
				case utf16:
					res.append((uint32_t)str16[i]);
					break;
				case utf8:
					res.append(fromUTF8(str+i));
					i += lenFromUTF8(str+i) - 1;
					break;
				default:
					res.append((uint32_t)str[i]);
				}
			}

			data.replace(streamIndex, res.length(), res);
			streamIndex += res.length();
		}

		uint8_t stringStream::get()
		{
			return data[streamIndex++];
		}

		uint8_t stringStream::peek()
		{
			return data[streamIndex];
		}

		uint32_t stringStream::getChar()
		{
			return data[streamIndex++];
		}

		bool stringStream::empty()
		{
			return streamIndex >= data.length();
		}

		bool stringStream::good()
		{
			return true;
		}

		bool stringStream::bad()
		{
			return false;
		}

		bool stringStream::binary()
		{
			return false;
		}

		bool stringStream::seekable()
		{
			return true;
		}

		void stringStream::seek(size_t index)
		{
			streamIndex = index;
			if (streamIndex > data.length()) streamIndex = data.length();
		}

		size_t stringStream::tell()
		{
			return streamIndex;
		}

		size_t stringStream::end()
		{
			return data.length();
		}

		encoding stringStream::format()
		{
			return utf32;
		}

		void stringStream::setFormat(encoding enc, bool force)
		{
			//can't change the encoding, will always be utf32.
			(void)enc;
			(void)force;
		}

		void stringStream::flush() {}
	}
}
