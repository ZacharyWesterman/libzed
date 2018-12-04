#include "inputStream.h"
#include <z/core/charFunctions.h>

#define MAX_BYTE_READ 32

namespace z
{
	namespace file
	{
		inputStream::inputStream()
		{
			initialized = false;
		}

		inputStream::inputStream(const zpath& fileName)
		{
			filestream.open((const char*)fileName.cstring(), std::ios::in);
			initialized = false;
		}

		void inputStream::open(const zpath& fileName)
		{
			filestream.close();
			filestream.open((const char*)fileName.cstring(), std::ios::in);
			initialized = false;
		}

		void inputStream::close()
		{
			filestream.close();
			initialized = false;
		}

		uint8_t inputStream::get()
		{
			return filestream.get();
		}

		uint32_t inputStream::getChar(encoding format)
		{
			if (format == utf32)
			{
				uint32_t result;
				uint8_t* buf = (uint8_t*)&result;

				//If the file's endianness != system endianness, swap byte order
#				if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
				if (endianness == __ORDER_LITTLE_ENDIAN__)
				{
					buf[3] = filestream.get();
					buf[2] = filestream.get();
					buf[1] = filestream.get();
					buf[0] = filestream.get();
				}
#				else
				if (endianness == __ORDER_BIG_ENDIAN__)
				{
					buf[3] = filestream.get();
					buf[2] = filestream.get();
					buf[1] = filestream.get();
					buf[0] = filestream.get();
				}
#				endif
				else
				{
					buf[0] = filestream.get();
					buf[1] = filestream.get();
					buf[2] = filestream.get();
					buf[3] = filestream.get();
				}
				return result;
			}
			else if (format == utf16)
			{
				uint16_t result;
				uint8_t* buf = (uint8_t*)&result;

				//If the file's endianness != system endianness, swap byte order
#				if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
				if (endianness == __ORDER_LITTLE_ENDIAN__)
				{
					buf[1] = filestream.get();
					buf[0] = filestream.get();
				}
#				else
				if (endianness == __ORDER_BIG_ENDIAN__)
				{
					buf[1] = filestream.get();
					buf[0] = filestream.get();
				}
#				endif
				else
				{
					buf[0] = filestream.get();
					buf[1] = filestream.get();
				}
				return result;
			}
			else
			{
				return filestream.get();
			}
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
			if (bad()) return ascii;
			if (initialized) return streamFormat;

			endianness = __BYTE_ORDER__;

			size_t init_pos = tell();
			size_t read_max = end() - init_pos;
			read_max = (MAX_BYTE_READ > read_max) ? read_max : MAX_BYTE_READ;

			bool has_null = false;
			bool can_ascii = true;

			uint8_t buf[4];
			int b_i = 0;

			for (size_t i=0; i<read_max; i++)
			{
				uint8_t ch = get();
				if (ch > 127) can_ascii = false;

				if (b_i >= 4)
				{
					//Read the BOM if it exists. if it has one, consume the BOM so the user does not read it.
					if (tell() <= (init_pos + 4))
					{
						if ((buf[0] == 0xFE) && (buf[1] == 0xFF)) //FE FF
						{
							endianness = __ORDER_BIG_ENDIAN__;
							return streamFormat = utf16;
						}
						else if ((buf[0] == 0xFF) && (buf[1] == 0xFE)) //FF FE ?? ??
						{
							endianness = __ORDER_LITTLE_ENDIAN__;

							if ((buf[2] == 0xFF) && (buf[3] == 0xFE)) //FF FE
							{
								seek(init_pos+2);
								return streamFormat = utf16;
							}
							else if (!(buf[2] | buf[3])) //FF FE 00 00
							{
								return streamFormat = utf32;
							}
						}
						else if (!(buf[0] | buf[1]) && (buf[2] == 0xFE) && (buf[3] == 0xFF)) //00 00 FE FF
						{
							endianness = __ORDER_BIG_ENDIAN__;
							return streamFormat = utf32;
						}
					}

					//last 4 chars, each byte is false if that char is null.
					uint8_t key = ((char)buf[0] << 3) + ((char)buf[1] << 2) + ((char)buf[2] << 1) + (char)buf[3];

					if (!key)//0000
					{
						initialized = true;
						seek(init_pos);
						return streamFormat = ascii;
					}
					else if (!(key & 0xC))//00..
					{
						initialized = true;
						endianness = __ORDER_BIG_ENDIAN__;
						seek(init_pos);
						return streamFormat = utf32;
					}
					else if (!(key & 0x3))//..00
					{
						initialized = true;
						endianness = __ORDER_LITTLE_ENDIAN__;
						seek(init_pos);
						return streamFormat = utf32;
					}
					else if (!(key & 0x8) || !(key & 0x2))//0... or ..0.
					{
						endianness = __ORDER_BIG_ENDIAN__;
						has_null = true;
					}
					else if (!(key & 0x4) || !(key & 0x1))//.0.. or ...0
					{
						endianness = __ORDER_LITTLE_ENDIAN__;
						has_null = true;
					}

					b_i = 0;
				}
				else
				{
					buf[b_i++] = ch;
				}
			}

			initialized = true;
			seek(init_pos);
			if (has_null) return streamFormat = utf16;
			return streamFormat = (can_ascii ? ascii : utf8);
		}
	}
}
