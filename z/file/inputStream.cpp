#include "inputStream.hpp"
#include <z/core/charFunctions.hpp>

#define MAX_BYTE_READ 32

#include <string.h>
#include <utility> //for std::swap

namespace z
{
	namespace file
	{
		inputStream::inputStream()
		{
			initialized = false;
			endianness_ = __BYTE_ORDER__;
			streamFormat = ascii;
		}

		inputStream::inputStream(const zpath& fileName)
		{
			filestream.open((const char*)fileName.cstring(), std::ios::in | std::ios::binary);
			initialized = false;
			endianness_ = __BYTE_ORDER__;
			streamFormat = ascii;
		}

		void inputStream::open(const zpath& fileName)
		{
			filestream.close();
			filestream.open((const char*)fileName.cstring(), std::ios::in | std::ios::binary);
			initialized = false;
		}

		void inputStream::close()
		{
			filestream.close();
			initialized = false;
		}

		uint8_t inputStream::get()
		{
			char ch = 0;
			filestream.read(&ch, 1);
			return ch;
		}

		uint32_t inputStream::getChar()
		{
			if (empty()) return 0;
			auto fmt = format();
			if (fmt == utf32)
			{
				uint32_t result;
				char* buf = (char*)&result;

				//If the file's endianness != system endianness, swap byte order
				filestream.read(buf, 4);
				if (endianness_ != __BYTE_ORDER__)
				{
					std::swap(buf[0], buf[3]);
					std::swap(buf[1], buf[2]);
				}

				return result;
			}
			else if (fmt == utf16)
			{
				uint16_t result = 0;
				char* buf = (char*)&result;

				//If the file's endianness != system endianness, swap byte order
				filestream.read(buf, 2);
				if (endianness_ != __BYTE_ORDER__)
				{
					std::swap(buf[0], buf[1]);
				}

				return result;
			}
			else
			{
				return get();
			}
		}

		void inputStream::seek(size_t index)
		{
			filestream.clear();
			filestream.seekg(index, filestream.beg);
		}

		size_t inputStream::tell()
		{
			return filestream.tellg();
		}

		bool inputStream::empty()
		{
			return filestream.eof() || filestream.bad();
		}

		bool inputStream::good()
		{
			return filestream.good() && filestream.is_open();
		}

		bool inputStream::bad()
		{
			return filestream.bad() || !good();
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

			endianness_ = __BYTE_ORDER__;

			size_t init_pos = tell();
			size_t read_max = end() - init_pos;
			read_max = (MAX_BYTE_READ > read_max) ? read_max : MAX_BYTE_READ;

			bool has_null = false;
			bool can_ascii = true;

			uint8_t buf[4];
			int b_i = 0;

			//Read the BOM if it exists. if it has one, consume the BOM so the user does not read it.
			uint8_t BOM[] = {0xFF, 0xFE, 0x00, 0x00};
			for (b_i=0; b_i<4; ++b_i)
			{
				buf[b_i] = get();
				if (buf[b_i] == BOM[b_i])
				{
					endianness_ = __ORDER_LITTLE_ENDIAN__;
				}
				else if (buf[b_i] == BOM[4-b_i])
				{
					endianness_ = __ORDER_BIG_ENDIAN__;
				}
				else
				{
					break;
				}
			}

			if (b_i == 2)
			{
				initialized = true;
				seek(2);
				return streamFormat = utf16;
			}
			else if (b_i == 4)
			{
				initialized = true;
				seek(4);
				return streamFormat = utf32;
			}
			seek(0);

			for (size_t i=0; i<read_max; i++)
			{
				uint8_t ch = get();
				if (ch > 127) can_ascii = false;

				if (b_i >= 4)
				{
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
						endianness_ = __ORDER_BIG_ENDIAN__;
						seek(init_pos);
						return streamFormat = utf32;
					}
					else if (!(key & 0x3))//..00
					{
						initialized = true;
						endianness_ = __ORDER_LITTLE_ENDIAN__;
						seek(init_pos);
						return streamFormat = utf32;
					}
					else if (!(key & 0x8) || !(key & 0x2))//0... or ..0.
					{
						endianness_ = __ORDER_BIG_ENDIAN__;
						has_null = true;
					}
					else if (!(key & 0x4) || !(key & 0x1))//.0.. or ...0
					{
						endianness_ = __ORDER_LITTLE_ENDIAN__;
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
			filestream.clear();
			if (has_null) return streamFormat = utf16;
			return streamFormat = (can_ascii ? ascii : utf8);
		}

		void inputStream::setFormat(encoding enc, bool force)
		{
			format();
			if (force) streamFormat = enc;
		}

		size_t inputStream::endianness()
		{
			return endianness_;
		}
	}
}
