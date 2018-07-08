#pragma once

#include "string.h"
#include <z/int.h>
#include <z/byte.h>

namespace z
{
	namespace core
	{
		/**
		 * \interface inputStream
		 * \brief A template class for character input streams.
		 *
		 * This class provides an interface for
		 * input streams.
		 */
		template <encoding E>
		class inputStream
		{
		public:
			/**
			 * \brief Virtual destructor.
			 */
			virtual ~inputStream() {}

			/**
			 * \brief Get the next character on the stream.
			 *
			 * Increments the stream index.
			 *
			 * \return The character previously at the beginning of the stream.
			 */
			virtual uint32_t get() = 0;

			/**
			 * \brief Get the next byte from the stream.
			 *
			 * Increments the stream index.
			 *
			 * \return The byte previously at the beginning of the stream.
			 */
			virtual uint8_t getByte() = 0;

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
			virtual string<E> get(size_t count) = 0;

			/**
			 * \brief Put the last removed character back on the stream.
			 *
			 * This is implementation-specific, but most streams should be
			 * able to put every character back on the stream.
			 */
			virtual void unget() = 0;

			/**
			 * \brief Read until a delimiter is encountered.
			 *
			 * If a non-null delimiter is specified, the stream
			 * should read until that delimiter is encountered,
			 * ignoring repetitions of the delimiter. If no
			 * delimiter or delimiter of \b null given, the
			 * stream should read until any whitespace is encountered,
			 * skipping repeating whitespace.
			 *
			 * \param delim an optional parameter indicating the
			 * delimiter to read to.
			 *
			 * \return A string containing all characters before
			 * the first occurrence of the given delimiter.
			 */
			virtual string<E> read(uint32_t delim = 0) = 0;

			/**
			 * \brief Tell whether we have consumed the last character in the stream.
			 *
			 * \return True if the last character has been consumed. False otherwise.
			 */
			virtual bool empty() = 0;

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
		template <encoding E>
		class outputStream
		{
		public:
			/**
			 * \brief Virtual destructor.
			 */
			virtual ~outputStream() {}

			/**
			 * \brief Add a character to the stream.
			 *
			 * Adds a character to the current index in the stream,
			 * incrementing the stream index.
			 *
			 * \param ch the character to add to the stream.
			 */
			virtual void put(uint32_t ch) = 0;

			/**
			 * \brief Add a byte to the stream.
			 *
			 * Adds a character to the current index in the stream,
			 * incrementing the stream index.
			 *
			 * \param ch the character to add to the stream.
			 */
			virtual void putByte(byte) = 0;

			/**
			 * \brief Write the given string to the stream.
			 *
			 * \param input the character string to append.
			 */
			virtual void write(const string<E>& input) = 0;

			/**
			 * \brief Write the given string to the stream, appending a newline.
			 *
			 * \param input the character string to append.
			 */
			void writeln(const string<E>& input);

			/**
			 * \brief Write a newline to the stream.
			 */
			void writeln();

			/**
			 * \brief Tell whether we have consumed the last character in the stream.
			 *
			 * \return True if the last character has been consumed. False otherwise.
			 */
			virtual bool empty() = 0;

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
		 * \brief Write the given string to the stream, then add a new line.
		 *
		 * Appends the given string to the stream. Then, adds a newline character.
		 *
		 * \param input the character string to append.
		 */
        template <encoding E>
        void outputStream<E>::writeln(const string<E>& input)
        {
            this->write(input);
            this->write("\n");
        }

		template <encoding E>
		void outputStream<E>::writeln()
		{
			this->write("\n");
		}

        /**
		 * \interface stream
         * \brief A template class for character streams.
         *
         * This class provides an interface for both input
		 * and output streams.
         */
        template <encoding E>
        class stream : public inputStream<E>, public outputStream<E>
        {};
    }
}
