#pragma once

#include <z/encoding.h>
#include "string.h"

namespace z
{
	namespace core
	{
		template <encoding E>
		class string;

		/**
		 * \interface inputStream
		 * \brief A template class for character input streams.
		 *
		 * This class provides an interface for
		 * input streams.
		 */
		class inputStream
		{
		public:
			/**
			 * \brief Virtual destructor.
			 */
			virtual ~inputStream() {}

			/**
			 * \brief Get the next byte on the stream.
			 *
			 * Increments the stream index.
			 *
			 * \return The character previously at the beginning of the stream.
			 */
			virtual uint8_t get() = 0;

			virtual uint32_t getChar(encoding format = ascii) = 0;

			/**
			 * \brief Pull an exact number of characters off the stream.
			 *
			 * Note that because of how the string class works, any
			 * \b null characters will be concatenated. Use CHAR get()
			 * if you are reading specially formatted data.
			 *
			 * \param count the number of characters to read.
			 *
			 * \return A string containing up to \b count non-null characters.
			 */
			virtual size_t get(size_t count, uint8_t* buf, encoding format = ascii) = 0;

			/**
			 * \brief Tell whether we have consumed the last character in the stream.
			 *
			 * \return True if the last character has been consumed. False otherwise.
			 */
			virtual bool empty() = 0;

			virtual bool good() = 0;
			virtual bool bad() = 0;
			virtual bool binary() = 0;

			/**
			 * \brief Seek a position in the stream.
			 *
			 * Seeks to the given index, starting at the beginning of the stream.
			 *
			 * \param index the position to seek.
			 */
			virtual void seek(size_t index) = 0;

			/**
			 * \brief Give the current position in the stream.
			 *
			 * \return The current position in the stream, starting
			 * at the beginning.
			 */
			virtual size_t tell() = 0;

			/**
			 * \brief Get the end of the stream.
			 *
			 * Gives the position after the last
			 * character in the stream.
			 *
			 * \return The end position of the character stream.
			 */
			virtual size_t end() = 0;
		};

		/**
		 * \interface outputStream
		 * \brief A template class for character output streams.
		 *
		 * This class provides an interface for
		 * output streams.
		 */
		class outputStream
		{
		public:
			/**
			 * \brief Virtual destructor.
			 */
			virtual ~outputStream() {}

			/**
			 * \brief Add a byte to the stream.
			 *
			 * Adds a byte to the current index in the stream,
			 * incrementing the stream index.
			 *
			 * \param ch the character to add to the stream.
			 */
			virtual void put(uint8_t ch) = 0;

			virtual void put(uint8_t* str, size_t count, encoding format = ascii) = 0;

			/**
			 * \brief Tell whether we have consumed the last character in the stream.
			 *
			 * \return True if the last character has been consumed. False otherwise.
			 */
			virtual bool empty() = 0;

			virtual bool good() = 0;
			virtual bool bad() = 0;
			virtual bool binary() = 0;

			/**
			 * \brief Seek a position in the stream.
			 *
			 * Seeks to the given index, starting at the beginning of the stream.
			 *
			 * \param index the position to seek.
			 */
			virtual void seek(size_t index) = 0;

			/**
			 * \brief Give the current position in the stream.
			 *
			 * \return The current position in the stream, starting
			 * at the beginning.
			 */
			virtual size_t tell() = 0;

			/**
			 * \brief Get the end of the stream.
			 *
			 * Gives the position after the last
			 * character in the stream.
			 *
			 * \return The end position of the character stream.
			 */
			virtual size_t end() = 0;
		};

		/**
		 * \interface stream
         * \brief A template class for character streams.
         *
         * This class provides an interface for both input
		 * and output streams.
         */
        class stream : public inputStream, public outputStream
        {};
	}
}
