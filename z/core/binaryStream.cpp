#include "binaryStream.hpp"
#include "charFunctions.hpp"

namespace z
{
	namespace core
	{
		binaryStream::binaryStream() : streamIndex(0)
		{
			data.clear();
		}

		binaryStream::~binaryStream()
		{
			data.clear();
		}

		void binaryStream::put(uint8_t ch)
		{
			data.add(ch);
		}

		void binaryStream::put(uint8_t* str, int count, encoding format)
		{
			if (!str) return;
			int datact;

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

			for (int i=0; i<datact; i++)
			{
				data.add(str[i]);
			}
		}

		uint8_t binaryStream::get()
		{
			if (streamIndex < (size_t)data.length())
				return data[streamIndex++];
			else
				return 0;
		}

		uint32_t binaryStream::getChar()
		{
			if (streamIndex >= (size_t)data.length()) return 0;

			int datact;

			uint8_t res8 = 0;
			uint16_t res16 = 0;
			uint32_t res32 = 0;
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

			if (streamIndex+datact > (size_t)data.length())
			{
				streamIndex = data.length();
				return 0;
			}

			for (int i=0; i<datact; i++)
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

		bool binaryStream::empty()
		{
			return (streamIndex >= (size_t)data.length());
		}

		bool binaryStream::good()
		{
			return true;
		}

		bool binaryStream::bad()
		{
			return false;
		}

		bool binaryStream::binary()
		{
			return true;
		}

		bool binaryStream::seekable()
		{
			return true;
		}

		void binaryStream::seek(size_t index)
		{
			if (index > (size_t)data.length())
				streamIndex = data.length();
			else
				streamIndex = index;
		}

		size_t binaryStream::tell()
		{
			return streamIndex;
		}

		size_t binaryStream::end()
		{
			return data.length();
		}

		encoding binaryStream::format()
		{
			if (initialized) return streamFormat;
			initialized = true;

			if (this->empty()) return streamFormat = ascii;

			int read_max = (32 > data.length()) ? data.length() : 32;

			int contig_nulls = 0;
			bool found_nulls = false;
			bool can_utf8 = true;

			for (int i=0; i<read_max; i++)
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
						if (!core::isUTF8((uint8_t*)&data[i], data.length()-i))
							can_utf8 = false;
					}
				}
			}

			if (found_nulls) return streamFormat = utf16;

			return streamFormat = (can_utf8 ? utf8 : ascii);
		}

		void binaryStream::setFormat(encoding enc, bool force)
		{
			if (!initialized || force)
			{
				streamFormat = enc;
				initialized = true;
			}
		}

		void binaryStream::flush() {}
	}
}
