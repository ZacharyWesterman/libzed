#pragma once

#include <type_traits>
#include <cstdint>
#include <cstddef>

#include "stream.h"

namespace z
{
	namespace core
	{

		/**
		 * \interface serializable
		 * \brief An interface for reading and writing object binary data to streams.
		 */
		class serializable
		{
		public:
			///Virtual destructor
			virtual ~serializable() {}

			/**
			 * \brief Read the object's binary data from a stream.
			 *
			 * \param stream The stream to read from.
			 */
			virtual void serialIn(inputStream& stream) = 0;

			/**
			 * \brief Write the object's binary data to a stream.
			 *
			 * \param stream The stream to write to.
			 */
			virtual void serialOut(outputStream& stream) const = 0;
		};

		/**
		 * \brief Read a serializable object as binary data from a stream.
		 *
		 * Calls the object's serialIn() member method.
		 *
		 * \param object The object to read from the stream, returned by reference.
		 * \param stream The stream to read from.
		 */
		template <typename T>
		typename std::enable_if<std::is_base_of<z::core::serializable, T>::value>::type
		serialIn(T& object, inputStream& stream)
		{
			object.serialIn(stream);
		}

		/**
		 * \brief Read a non-serializable object or datatype as binary data from a stream.
		 *
		 * \param object The object to read from the stream, returned by reference.
		 * \param stream The stream to read from.
		 */
		template <typename T>
		typename std::enable_if<!std::is_base_of<z::core::serializable, T>::value && !std::is_integral<T>::value>::type
		serialIn(T& object, inputStream& stream)
		{
			size_t length = sizeof(T);
			uint8_t* data = (uint8_t*)&object;

			for (size_t i=0; i<length; i++)
			{
				data[i] = stream.get();
			}
		}

		/**
		 * \brief Read an integer as binary data from a stream.
		 *
		 * This function assumes integers are no larger than 255 bytes in size.
		 * The first byte is assumed to be the number of bytes to read afterward.
		 * Then that number of bytes is read from the stream.
		 *
		 * \param number The number to read from the stream, returned by reference.
		 * \param stream The stream to read from.
		 */
		template <typename T>
		typename std::enable_if<std::is_integral<T>::value>::type
		serialIn(T& number, inputStream& stream)
		{
			if (stream.bad() || !stream.binary()) return;

			uint8_t chars = stream.get();
			number = 0;

			for (size_t i=0; i<chars; i++)
			{
				number <<= 8;
				number += stream.get();
			}
		}

		/**
		* \brief Write a serializable object as binary data to a stream.
		*
		* Calls the object's serialOut() member method.
		*
		* \param object The object to write to the stream.
		* \param stream The stream to write to.
		*/
		template <typename T>
		typename std::enable_if<std::is_base_of<z::core::serializable, T>::value>::type
		serialOut(const T& object, outputStream& stream)
		{
			object.serialOut(stream);
		}

		/**
		* \brief Write a non-serializable object or datatype as binary data to a stream.
		*
		* \param object The object to write to the stream.
		* \param stream The stream to write to.
		*/
		template <typename T>
		typename std::enable_if<!std::is_base_of<z::core::serializable, T>::value && !std::is_integral<T>::value>::type
		serialOut(const T& object, outputStream& stream)
		{
			size_t length = sizeof(T);
			uint8_t* data = (uint8_t*)&object;

			for (size_t i=0; i<length; i++)
			{
				stream.put(data[i]);
			}
		}

		/**
		 * \brief Write an integer as binary data to a stream.
		 *
		 * This function assumes integers are no larger than 255 bytes in size.
		 * Leading null bytes are ignored and then the count of remaining bytes
		 * is written to the stream. Then all remaining bytes are written.
		 *
		 * \param number The number to write to the stream.
		 * \param stream The stream to write to.
		 */
		template <typename T>
		typename std::enable_if<std::is_integral<T>::value>::type
		serialOut(T number, outputStream& stream)
		{
			if (stream.bad() || !stream.binary()) return;

			uint8_t c[sizeof(T)];
			uint8_t chars = 0;

			for (size_t i=0; i<sizeof(T); i++)
			{
				if (number)
				{
					chars++;
					c[i] = number & 0xFF;
					number >>= 8;
				}
				else break;
			}

			stream.put(chars);
			for (size_t i=0; i<chars; i++)
				stream.put(c[i]);
		}

	}
}
