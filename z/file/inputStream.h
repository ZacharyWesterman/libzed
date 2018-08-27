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

		public:
			/**
			 * \brief Constructor
			 *
			 * As this is a file stream, it must be constructed with a file name.
			 *
			 * \param fileName a string containing the name of the file to read from.
			 */
			inputStream(const core::string<utf8>&);

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