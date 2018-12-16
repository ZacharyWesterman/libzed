#include "outputStream.h"
#include <z/core/charFunctions.h>

#include "inputStream.h"

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
						filestream.put(0x00);
						filestream.put(0x00);
						filestream.put(0xFE);
						filestream.put(0xFF);
					}
					else
					{
						filestream.put(0xFF);
						filestream.put(0xFE);
						filestream.put(0x00);
						filestream.put(0x00);
					}
				}
				else if (streamFormat == utf16)
				{
					if (endianness_ == __ORDER_BIG_ENDIAN__)
					{
						filestream.put(0xFE);
						filestream.put(0xFF);
					}
					else
					{
						filestream.put(0xFF);
						filestream.put(0xFE);
					}
				}
			}

			for (size_t i=0; i<count; i++)
			{
				if (format == utf32)
				{
// 					size_t index = i << 2;
// 					uint32_t ch = *((uint32_t*)&str[index]);
// 					// uint8_t* buf = (uint8_t*)&ch;
// 					if (streamFormat == utf16)
// 					{
//
// 					}
//
// #					if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
// 					if (endianness_ == __ORDER_LITTLE_ENDIAN__)
// 					{
// 						buf[0] = str[index+3];
// 						buf[1] = str[index+2];
// 						buf[2] = str[index+1];
// 						buf[3] = str[index];
// 					}
// #					else
// 					if (endianness_ == __ORDER_BIG_ENDIAN__)
// 					{
// 						buf[0] = str[index+3];
// 						buf[1] = str[index+2];
// 						buf[2] = str[index+1];
// 						buf[3] = str[index];
// 					}
// #					endif
// 					else
// 					{
// 						buf[0] = str[index];
// 						buf[1] = str[index+1];
// 						buf[2] = str[index+2];
// 						buf[3] = str[index+3];
// 					}
//
// 					if (streamFormat == utf16)
// 					{
// 						uint16_t = ch16;
// 						buf = (uint16_t*)&ch16;
// 						if (ch > 0xFFFF)
// 						{
//
// 						}
// 					}
				}
				else if (format == utf16)
				{
					size_t index = i << 1;

#					if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
					if (endianness_ == __ORDER_LITTLE_ENDIAN__)
					{
						filestream.put(str[index+1]);
						filestream.put(str[index]);
					}
#					else
					if (endianness_ == __ORDER_BIG_ENDIAN__)
					{
						filestream.put(str[index+1]);
						filestream.put(str[index]);
					}
#					endif
					else
					{
						filestream.put(str[index]);
						filestream.put(str[index+1]);
					}
				}
				else
				{
					filestream.put(str[i]);
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
			return streamFormat;
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
