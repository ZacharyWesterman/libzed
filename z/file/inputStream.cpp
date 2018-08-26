#include "inputStream.h"

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
	}
}
