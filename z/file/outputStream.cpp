#include "outputStream.hpp"
#include <z/core/charFunctions.hpp>

#include "inputStream.hpp"

namespace z
{
	namespace file
	{
		outputStream::outputStream()
		{
			initialized = false;
		}

		outputStream::outputStream(const zpath& fileName, encoding fileFormat, bool append)
		{
			initialized = true;

			if (append)
			{
				inputStream tempStream (fileName);
				if (tempStream.good())
				{
					streamFormat = tempStream.format();
					endianness_ = tempStream.endianness();
					tempStream.close();

					filestream.open((const char*)fileName.cstring(), std::ios::app | std::ios::out);
					return;
				}
			}

			endianness_ = __BYTE_ORDER__;
			streamFormat = fileFormat;
			filestream.open((const char*)fileName.cstring(), std::ios::out);
		}

		void outputStream::open(const zpath& fileName, encoding fileFormat, bool append)
		{
			filestream.close();

			initialized = true;

			if (append)
			{
				inputStream tempStream (fileName);
				if (tempStream.good())
				{
					streamFormat = tempStream.format();
					endianness_ = tempStream.endianness();
					tempStream.close();

					filestream.open((const char*)fileName.cstring(), std::ios::app | std::ios::out);
					return;
				}
			}

			endianness_ = __BYTE_ORDER__;
			streamFormat = fileFormat;
			filestream.open((const char*)fileName.cstring(), std::ios::out);
		}

		void outputStream::close()
		{
			filestream.close();
			initialized = false;
		}

		void outputStream::put(uint8_t ch)
		{
			filestream.put(ch);
		}

		void outputStream::put(uint8_t* str, size_t count, encoding format)
		{
			//write the BOM if we're at the beginning of the stream.
			if (!tell())
			{
				if (streamFormat == utf32)
				{
					if (endianness_ == __ORDER_BIG_ENDIAN__)
					{
						char buf[] = {0,0,(char)254,(char)255};
						filestream.write(buf,4);
					}
					else
					{
						char buf[] = {(char)255,(char)254,0,0};
						filestream.write(buf,4);
					}
				}
				else if (streamFormat == utf16)
				{
					if (endianness_ == __ORDER_BIG_ENDIAN__)
					{
						char buf[] = {(char)254,(char)255};
						filestream.write(buf,2);
					}
					else
					{
						char buf[] = {(char)255,(char)254};
						filestream.write(buf,2);
					}
				}
			}


			if ((format == utf8) || (format == ascii))
			{
				filestream.write((char*)str, count);
			}
			else
			{
				if (endianness_ == __BYTE_ORDER__)
				{
					if (format == utf32)
						filestream.write((char*)str, count << 2);
					else //(format == utf16)
						filestream.write((char*)str, count << 1);
				}
				else
				{
					char buf[4];
					size_t index;

					for (size_t i=0; i<count; i++)
					{
						if (format == utf32)
						{
							index = i << 2;
							buf[0] = str[index+3];
							buf[1] = str[index+2];
							buf[2] = str[index+1];
							buf[3] = str[index];
							filestream.write(buf,4);
						}
						else //(format == utf16)
						{
							index = i << 1;
							buf[0] = str[index+1];
							buf[1] = str[index];
							filestream.write(buf,2);
						}
					}
				}
			}
		}

		void outputStream::seek(size_t index)
		{
			filestream.seekp(index, filestream.beg);
		}

		size_t outputStream::tell()
		{
			return filestream.tellp();
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
			return streamFormat;
		}

		void outputStream::setFormat(encoding enc, bool force)
		{
			if (force || !tell()) streamFormat = enc;
		}

		void outputStream::flush()
		{
			filestream.flush();
		}

		size_t outputStream::endianness()
		{
			return endianness_;
		}
	}
}
