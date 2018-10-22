#pragma once

#include "stream.h"
#include <iostream>

namespace z
{
	namespace core
	{
		/**
		 * \brief A class for streaming directly to and from memory.
		 *
		 * This class provides an implementation of core::stream for raw data in memory.
		 */
		class memoryStream : public stream
		{
		private:
			uint8_t* data;
			size_t dataSize;
			size_t streamIndex;

		public:
			template<typename T>
			memoryStream(T* streamData = 0, size_t count = 1)
			{
				data = (uint8_t*)streamData;
				dataSize = data ? (sizeof(T)*count) : 0;
				streamIndex = 0;
			}

			template<typename T>
			void set(T* streamData, size_t count = 1)
			{
				data = (uint8_t*)streamData;
				dataSize = data;
			}

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

			encoding format();

			void flush();
		};
	}
}
