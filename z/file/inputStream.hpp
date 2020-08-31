#pragma once

#include <fstream>
#include <stdlib.h>
#include <cstdint>

#include <z/core/string.hpp>
#include <z/core/stream.hpp>

namespace z
{
	namespace file
	{
		/**
		 * \brief A class for file input streams.
		 *
		 * This class provides an implementation of core::inputStream for files.
		 */
		class inputStream : public core::inputStream
		{
		private:
			std::ifstream filestream;

			size_t endianness_;
			encoding streamFormat;
			bool initialized;

		public:
			/**
			 * \brief Empty constructor.
			 */
			inputStream();

			/**
			 * \brief Constructor
			 *
			 * As this is a file stream, it must be constructed with a file name.
			 *
			 * \param fileName a string containing the name of the file to read from.
			 */
			inputStream(const zpath& fileName);

			/**
			 * \brief Reopen a stream to the given file.
			 *
			 * \param fileName a string containing the name of the file to read from.
			 */
			void open(const zpath& fileName);

			///Close the current file stream.
			void close();

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

			/**
			 * \brief Get the endianness of this stream.
			 *
			 * return The determined endianness of the stream, either __ORDER_LITTLE_ENDIAN__ or __ORDER_BIG_ENDIAN__
			 */
			size_t endianness();
		};
	}
}
