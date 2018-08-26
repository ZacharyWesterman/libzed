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
		 * \brief A class for file output streams.
		 *
		 * This class provides an implementation of core::outputStream for files.
		 */
		class outputStream : public core::outputStream
		{
		private:
			std::ofstream filestream;

		public:
			/**
			 * \brief Constructor
			 *
			 * As this is a file stream, it must be constructed with a file name.
			 *
			 * \param fileName a string containing the name of the file to read from.
			 * \param append set to \b true to append to the file, \b false to overwrite.
			 */
			outputStream(const core::string<utf8>&, bool append = false);

			void put(uint8_t ch);
			void put(uint8_t* str, size_t count, encoding format = ascii);

			bool empty();
			bool good();
			bool bad();
			bool binary();
			bool seekable();

			void seek(size_t);
			size_t tell();
			size_t end();
		};
	}
}
