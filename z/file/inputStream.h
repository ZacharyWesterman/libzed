#pragma once

#include <fstream>
#include <stdlib.h>
#include <cstdint>

#include <z/core/string.h>
#include <z/core/stream.h>

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
			void setFormat(encoding enc, bool force = false);

			/**
			 * \brief Get the endianness of this stream.
			 *
			 * return The determined endianness of the stream, either __ORDER_LITTLE_ENDIAN__ or __ORDER_BIG_ENDIAN__
			 */
			size_t endianness();
		};
	}
}
