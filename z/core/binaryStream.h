#pragma once

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
			bool seekable();

 			void seek(size_t index);
 			size_t tell();
 			size_t end();
		};
	}
}
