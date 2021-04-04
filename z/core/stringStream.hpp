#pragma once

#include "string.hpp"
#include "stream.hpp"

namespace z
{
	namespace core
	{
		/**
		 * \brief A class for streaming directly to and from a z::core::string<z::utf32>.
		 *
		 * This class provides an implementation of core::stream for data in a string.
		 */
		class stringStream : public stream
		{
		private:
			zstring data;
			int streamIndex;

		public:
			/**
			 * \brief Generic constructor.
			 */
			template<typename T>
			stringStream() : streamIndex(0) {}

			void put(uint8_t ch) override;
			void put(uint8_t* str, int count, encoding format = ascii) override;

			uint8_t get() override;
			uint8_t peek() override;
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
