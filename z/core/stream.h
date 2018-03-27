#pragma once
#ifndef CORE_STREAM_H_INCLUDED
#define CORE_STREAM_H_INCLUDED

#include "string.h"
#include <z/int.h>

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
		template <typename CHAR>
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
			 * \return The character previously at the
			 * beginning of the stream.
			 */
			virtual CHAR get() = 0;

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
            virtual string<CHAR> get(Int count) = 0;

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
            virtual core::string<CHAR> read(CHAR delim = 0) = 0;

			/**
			 * \brief Tell whether we have consumed the last character in
			 * the stream.
			 *
			 * \return \b True if the last character has been consumed.
			 * \b False otherwise.
			 */
            virtual bool empty() = 0;

			/**
			 * \brief Seek a position in the stream.
			 *
			 * Seeks to the given index, starting at the
			 * beginning of the stream.
			 *
			 * \param index the position to seek.
			 */
            virtual void seek(Int index) = 0;

			/**
			 * \brief Give the current position in the stream.
			 *
			 * \return The current position in the stream, starting
			 * at the beginning.
			 */
            virtual Int tell() = 0;
        };

		/**
         * \brief A template class for character output streams.
         *
         * This class provides an interface for
		 * output streams.
         */
        template <typename CHAR>
        class outputStream
        {
        public:
            virtual ~outputStream() {}

            virtual void put(CHAR) = 0;
            virtual void write(const core::string<CHAR>&) = 0;
            void writeln(const core::string<CHAR>&);

            virtual bool empty() = 0;
            virtual void seek(Int) = 0;
            virtual Int tell() = 0;
        };

        template <typename CHAR>
        void outputStream<CHAR>::writeln(const core::string<CHAR>& input)
        {
            this->write(input);
            this->write("\n");
        }

        /**
         * \brief A template class for character streams.
         *
         * This class provides an interface for both input
		 * and output streams.
         */
        template <typename CHAR>
        class stream : public inputStream<CHAR>, public outputStream<CHAR>
        {
        public:
            virtual ~stream() {}
        };
    }
}

#endif // STREAM_H_INCLUDED
