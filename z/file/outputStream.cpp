#include "outputStream.h"
#include <z/core/charFunctions.h>

namespace z
{
	namespace file
	{
		outputStream::outputStream(const core::string<utf8>& fileName, bool append)
		{
			if (append)
				filestream.open((const char*)fileName.cstring(), std::ios::app | std::ios::out);
			else
				filestream.open((const char*)fileName.cstring(), std::ios::out);
		}

		void outputStream::put(uint8_t ch)
		{
			filestream.put(ch);
		}

		void outputStream::put(uint8_t* str, size_t count, encoding format)
		{
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

			for (size_t i=0; i<datact; i++)
				filestream.put(str[i]);
		}

		void outputStream::seek(size_t index)
		{
			filestream.seekp(index, filestream.beg);
		}

		size_t outputStream::tell()
		{
			return filestream.tellp();
		}

		bool outputStream::empty()
		{
			return filestream.eof();
		}

		bool outputStream::good()
		{
			return filestream.good() && filestream.is_open();
		}

		bool outputStream::bad()
		{
			return filestream.bad() || !filestream.is_open();
		}

		bool outputStream::binary()
		{
			return true;
		}

		bool outputStream::seekable()
		{
			return true;
		}

		size_t outputStream::end()
		{
			return filestream.end - filestream.beg;
		}

		encoding outputStream::format()
		{
			return ascii;
		}
	}
}
