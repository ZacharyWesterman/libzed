#include "inputStream.h"
#include <z/core/charFunctions.h>

namespace z
{
	namespace file
	{
		inputStream::inputStream(const core::string<utf8>& fileName)
		{
			filestream.open((const char*)fileName.cstring(), std::ios::in);
		}

		uint8_t inputStream::get()
		{
			return filestream.get();
		}

		uint32_t inputStream::getChar(encoding format)
		{
			uint32_t result;

			switch (format)
			{
				case utf16:
					result = filestream.get();
					result = (result << 8) + filestream.get();
					break;

				case utf32:
					result = filestream.get();
					result = (result << 8) + filestream.get();
					result = (result << 8) + filestream.get();
					result = (result << 8) + filestream.get();
					break;

				default:
					result = filestream.get();
			}

			return result;
		}

		void inputStream::seek(size_t index)
		{
			filestream.seekg(index, filestream.beg);
		}

		size_t inputStream::tell()
		{
			return filestream.tellg();
		}

		bool inputStream::empty()
		{
			return filestream.eof();
		}

		bool inputStream::good()
		{
			return filestream.good() && filestream.is_open();
		}

		bool inputStream::bad()
		{
			return filestream.bad() || !filestream.is_open();
		}

		bool inputStream::binary()
		{
			return true;
		}

		bool inputStream::seekable()
		{
			return true;
		}

		size_t inputStream::end()
		{
			return filestream.end - filestream.beg;
		}

		encoding inputStream::format()
		{
			if (this->empty()) return ascii;

			size_t read_max = (32 > this->end()) ? this->end() : 32;

			size_t max_nulls = 0;
			size_t contig_nulls = 0;
			bool can_utf8 = true;

			size_t init_pos = this->tell();
			this->seek(0);

			uint8_t buffer[read_max];
			for (size_t i=0; i<read_max; i++)
				buffer[i] = this->get();

			this->seek(init_pos);

			for (size_t i=0; i<read_max; i++)
			{
				//ascii and utf8 won't have null chars
				if (max_nulls || !buffer[i])
				{
					if (contig_nulls >= 2) return utf32;

					contig_nulls++;
					if (max_nulls < contig_nulls) max_nulls = contig_nulls;
				}
				else if (can_utf8)
				{
					if (!core::isUTF8(&buffer[i], read_max-i))
						can_utf8 = false;
				}
			}

			if (max_nulls) return utf16;

			return can_utf8 ? utf8 : ascii;
		}
	}
}
