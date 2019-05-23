#include "memoryStream.h"
#include "charFunctions.h"
namespace z
{
	namespace core
	{
		void memoryStream::put(uint8_t ch)
		{
			if (!data) return;

			if (streamIndex < dataSize)
				data[streamIndex++] = ch;
		}

		void memoryStream::put(uint8_t* str, size_t count, encoding format)
		{
			if (!(str && data)) return;
			size_t datact;

			switch (format)
			{
			case utf16:
				datact = count << 1;
				break;

			case utf32:
				datact = count << 2;
				break;

			default:
				datact = count;
			}

			if ((streamIndex + datact) > dataSize)
				datact = dataSize - streamIndex;

			for (size_t i=0; i<datact; i++)
			{
				data[streamIndex++] = str[i];
			}
		}

		uint8_t memoryStream::get()
		{
			if (streamIndex < dataSize)
				return data[streamIndex++];
			else
				return 0;
		}

		uint32_t memoryStream::getChar()
		{
			if (streamIndex > dataSize) return 0;

			size_t datact;

			uint8_t res8;
			uint16_t res16;
			uint32_t res32;
			uint8_t* str;

			switch (format())
			{
			case utf16:
				datact = 2;
				str = (uint8_t*)&res16;
				break;

			case utf32:
				datact = 4;
				str = (uint8_t*)&res32;
				break;

			default:
				datact = 1;
				str = (uint8_t*)&res8;
			}

			if ((streamIndex + datact) > dataSize)
			{
				streamIndex = dataSize+1;
				return 0;
			}

			for (size_t i=0; i<datact; i++)
				str[i] = data[streamIndex++];

			switch (format())
			{
				case utf16:
					return res16;
				case utf32:
					return res32;
				default:
					return res8;
			}
		}

		bool memoryStream::empty()
		{
			return (streamIndex > dataSize);
		}

		bool memoryStream::good()
		{
			return (data && dataSize);
		}

		bool memoryStream::bad()
		{
			return !(data && dataSize);
		}

		bool memoryStream::binary()
		{
			return true;
		}

		bool memoryStream::seekable()
		{
			return true;
		}

		void memoryStream::seek(size_t index)
		{
			streamIndex = (index > dataSize) ? dataSize : index;
		}

		size_t memoryStream::tell()
		{
			return streamIndex;
		}

		size_t memoryStream::end()
		{
			return dataSize;
		}

		encoding memoryStream::format()
		{
			if (initialized) return streamFormat;
			initialized = true;

			if (this->empty()) return streamFormat = ascii;

			size_t read_max = (32 > dataSize) ? dataSize : 32;

			size_t contig_nulls = 0;
			bool found_nulls = false;
			bool can_utf8 = true;

			for (size_t i=0; i<read_max; i++)
			{
				//ascii and utf8 won't have null chars
				if (found_nulls || !data[i])
				{
					found_nulls = true;
					if (contig_nulls >= 2) return streamFormat = utf32;

					contig_nulls++;
				}
				else if (!found_nulls)
				{
					contig_nulls = 0;
					if (can_utf8)
					{
						if (!core::isUTF8(&data[i], dataSize-i))
							can_utf8 = false;
					}
				}
			}

			if (found_nulls) return streamFormat = utf16;

			return streamFormat = (can_utf8 ? utf8 : ascii);
		}

		void memoryStream::setFormat(encoding enc, bool force)
		{
			if (!initialized || force)
			{
				streamFormat = enc;
				initialized = true;
			}
		}

		void memoryStream::flush() {}
	}
}
