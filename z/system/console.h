#pragma once

#include <z/encoding.h>
#include <z/core/stream.h>

namespace z
{
	namespace system
	{
		/**
		 * \brief Class for interacting with the console.
		 *
		 * This class provides an implementation of core::stream.
		 * All read-related methods read from stdin, and all
		 * write-related methods write to stdout. Note that
		 * because of how stdin and stdout behave, you cannot
		 * unget characters or seek a position in the stream;
		 * those are just dummy methods.
		 * As a result, this class is not compatible with the
		 * util::regex class.
		 */
		class console : public core::stream
 		{
 		public:
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
