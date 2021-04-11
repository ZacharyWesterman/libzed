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

			void put(uint8_t ch) override;
 			void put(uint8_t* str, int count, encoding format = ascii) override;

 			uint8_t get() override;
 			uint32_t getChar() override;

 			bool empty() override;
			bool good() override;
			bool bad() override;
			bool binary() override;
			bool seekable() override;

 			void seek(size_t index) override;
 			size_t tell() override;
 			size_t end() override;

			encoding format() override;
			void setFormat(encoding enc, bool force = false) override;

			void flush() override;
		};
	}
}
