#pragma once

#include <cstdint>
#include <cstddef>

#include <z/encoding.h>

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
		 * <br/><br/>
		 * <B>RE-ENTRANCE:</B><br/>
		 * Simultaneous accesses to the same object can cause data races.
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
			 * \return The byte previously at the beginning of the stream.
			 */
			virtual uint8_t get() = 0;

			/**
			 * \brief Get the next chracter on the stream in the given encoding.
			 *
			 * Increments the stream index.
			 *
			 * \param format The character encoding scheme.
			 *
			 * \return The character previously at the beginning of the stream.
			 */
			virtual uint32_t getChar(encoding format = ascii) = 0;

			/**
			 * \brief Tell whether we have consumed the last character in the stream.
			 *
			 * \return True if the last character has been consumed. False otherwise.
			 */
			virtual bool empty() = 0;

			/**
			 * \brief Tell whether this is a valid stream object.
			 *
			 * \return True if we can properly read to and write from the stream. False otherwise.
			 */
			virtual bool good() = 0;

			/**
			 * \brief Tell whether this is an invalid stream object.
			 *
			 * \return False if we can properly read to and write from the stream. True otherwise.
			 */
			virtual bool bad() = 0;

			/**
			 * \brief Tell whether this stream supports any binary format.
			 *
			 * \return True if this is a binary stream. False otherwise.
			 */
			virtual bool binary() = 0;

			/**
			 * \brief Tell whether this stream allows seeking to an index.
			 *
			 * \return True if seeking is possible for this stream. False otherwise.
			 */
			virtual bool seekable() = 0;

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

			/**
			 * \brief Get the expected format of the stream.
			 *
			 * Analyzes a portion of the stream in an attempt to determine how
			 * Characters on it are formatted. Note that this only gives an assumption of
			 * the most likely encoding so it may not be correct in some cases.
			 * Additionally results will be inaccurate if the stream contains
			 * raw binary data.
			 *
			 * \return The determined encoding format of the stream.
			 */
			 virtual encoding format() = 0;
		};

		/**
		 * \interface outputStream
		 * \brief A template class for character output streams.
		 *
		 * This class provides an interface for
		 * output streams.
		 * <br/><br/>
		 * <B>RE-ENTRANCE:</B><br/>
		 * Simultaneous accesses to the same object can cause data races.
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

			/**
			 * \brief Add a number of characters in the given format to the stream.
			 *
			 * Adds a number of characters, assumed to be in the given format, to the stream,
			 * incrementing the stream index.
			 *
			 * \param str Pointer to the characters to add to the stream.
			 * \param count The number of characters.
			 * \param format The scheme the characters are encoded in.
			 */
			virtual void put(uint8_t* str, size_t count, encoding format = ascii) = 0;

			/**
			 * \brief Tell whether we have consumed the last character in the stream.
			 *
			 * \return True if the last character has been consumed. False otherwise.
			 */
			virtual bool empty() = 0;

			/**
			 * \brief Tell whether this is a valid stream object.
			 *
			 * \return True if we can properly read to and write from the stream. False otherwise.
			 */
			virtual bool good() = 0;

			/**
			 * \brief Tell whether this is an invalid stream object.
			 *
			 * \return False if we can properly read to and write from the stream. True otherwise.
			 */
			virtual bool bad() = 0;

			/**
			 * \brief Tell whether this stream supports any binary format.
			 *
			 * \return True if this is a binary stream. False otherwise.
			 */
			virtual bool binary() = 0;

			/**
			 * \brief Tell whether this stream allows seeking to an index.
			 *
			 * \return True if seeking is possible for this stream. False otherwise.
			 */
			virtual bool seekable() = 0;

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

			/**
			 * \brief Get the expected format of the stream.
			 *
			 * Analyzes a portion of the stream in an attempt to determine how
			 * Characters on it are formatted. Note that this only gives an assumption of
			 * the most likely encoding so it may not be correct in some cases.
			 * Additionally results will be inaccurate if the stream contains
			 * raw binary data.
			 *
			 * \return The determined encoding format of the stream.
			 */
			virtual encoding format() = 0;

			 /**
			 * \brief Flush all data from the stream buffer.
			 *
			 * Writes any buffered stream data and clears the buffer.
			 */
			virtual void flush() = 0;
		};

		/**
		 * \interface stream
		 * \brief A template class for character streams.
		 *
		 * This class provides an interface for both input
		 * and output streams.
		 *
		 * <br/><br/>
		 * <B>RE-ENTRANCE:</B><br/>
		 * Simultaneous accesses to the same object can cause data races.
		 *
		 */
		class stream : public inputStream, public outputStream
		{};
	}
}
