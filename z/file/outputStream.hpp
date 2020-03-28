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
		 * \brief A class for file output streams.
		 *
		 * This class provides an implementation of core::outputStream for files.
		 */
		class outputStream : public core::outputStream
		{
		private:
			std::ofstream filestream;

			size_t endianness_;
			encoding streamFormat;

			bool initialized;

		public:
			outputStream();

			/**
			 * \brief Constructor
			 *
			 * As this is a file stream, it must be constructed with a file name.
			 *
			 * \param fileName a string containing the name of the file to read from.
			 * \param fileFormat the format to write to the file. If append is set and the file exists, then this parameter is ignored in favor of the file's original encoding.
			 * \param append set to \b true to append to the file, \b false to overwrite.
			 */
			outputStream(const zpath& fileName, encoding fileFormat = ascii, bool append = false);

			/**
			 * \brief Reopen the stream to the given file.
			 *
			 * \param fileName a string containing the name of the file to read from.
			 * \param fileFormat the format to write to the file. If append is set, then this parameter is ignored if the file already exists.
			 * \param append set to \b true to append to the file, \b false to overwrite.
			 */
			void open(const zpath& fileName, encoding fileFormat = ascii, bool append = false);

			///Close the current file stream.
			void close();

			void put(uint8_t ch);
			void put(uint8_t* str, size_t count, encoding format = ascii);

			bool good();
			bool bad();
			bool binary();
			bool seekable();

			void seek(size_t);
			size_t tell();
			size_t end();

			/**
			 * \brief Get the expected format of the stream.
			 *
			 * Analyzes a portion of the stream in an attempt to determine how
			 * Characters on it are formatted. Note that this only gives an assumption of
			 * the most likely encoding so it may not be correct in some cases.
			 * Additionally results will be inaccurate if the stream contains
			 * raw binary data.
			 *
			 * \return 0 (ascii), as the format of file output streams should be determined beforehand.
			 */
			encoding format();

			void setFormat(encoding enc, bool force = false);

			void flush();

			/**
			 * \brief Get the endianness of this stream.
			 *
			 * return The determined endianness of the stream, either __ORDER_LITTLE_ENDIAN__ or __ORDER_BIG_ENDIAN__
			 */
			size_t endianness();
		};
	}
}
