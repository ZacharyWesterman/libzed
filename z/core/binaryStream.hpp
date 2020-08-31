#pragma once

#include "array.hpp"
#include "stream.hpp"

namespace z
{
	namespace core
	{
		/**
		 * \brief A class for binary data streams.
		 *
		 * This class provides an implementation of core::stream for binary data in memory.
		 */
		class binaryStream : public stream
		{
		private:
			array<uint8_t> data;
			size_t streamIndex;
			encoding streamFormat;
			bool initialized;

		public:
			binaryStream();
			~binaryStream();

			void put(uint8_t ch);
 			void put(uint8_t* str, int count, encoding format = ascii);

 			uint8_t get();
 			uint32_t getChar();

 			bool empty();
			bool good();
			bool bad();
			bool binary();
			bool seekable();

 			void seek(size_t index) override;
 			size_t tell() override;
 			size_t end() override;

			encoding format();
			void setFormat(encoding enc, bool force = false);

			void flush();
		};
	}
}
