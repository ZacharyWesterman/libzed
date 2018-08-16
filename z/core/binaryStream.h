#pragma once
#ifndef BINARYSTREAM_H_INCLUDED
#define BINARYSTREAM_H_INCLUDED

#include "array.h"
#include "stream.h"

namespace z
{
	namespace core
	{
		/**
		 * \brief A class for bunary data streams.
		 *
		 * This class provides an implementation of core::stream for binary data in memory.
		 */
		class binaryStream : public stream
		{
		private:
			array<uint8_t> data;
			size_t streamIndex;

		public:
			binaryStream();

			void put(uint8_t ch);
 			void put(uint8_t* str, size_t count, encoding format = ascii);

 			uint8_t get();
 			uint32_t getChar(encoding format = ascii);

 			bool empty();
			bool good();
			bool bad();
			bool binary();

 			void seek(size_t index);
 			size_t tell();
 			size_t end();
		};

		binaryStream::binaryStream() : streamIndex(0) {}

		void binaryStream::put(uint8_t ch)
		{
			data.add(ch);
		}

		void binaryStream::put(uint8_t* str, size_t count, encoding format)
		{
			if (!str) return;
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
			{
				data.add(str[i]);
			}
		}

		uint8_t binaryStream::get()
		{
			if (streamIndex < data.length())
				return data[streamIndex++];
			else
				return 0;
		}

		uint32_t binaryStream::getChar(encoding format)
		{
			if (streamIndex >= data.length()) return 0;

			size_t datact;

			uint8_t res8;
			uint16_t res16;
			uint32_t res32;
			uint8_t* str;

			switch (format)
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

			if (streamIndex+datact > data.length())
			{
				streamIndex = data.length();
				return 0;
			}

			for (size_t i=0; i<datact; i++)
				str[i] = data[streamIndex++];

			switch (format)
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
			return (streamIndex >= data.length());
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

		void binaryStream::seek(size_t index)
		{
			if (index > data.length())
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
	}
}

#endif //BINARYSTREAM_H_INCLUDED
