#pragma once

#include <cstdint>

#include <z/z.h>
// #include "string.h"

#include "stream.h"

namespace z
{
	namespace core
	{

		class serializable
		{
		public:
			virtual ~serializable() {}

			virtual void serialIn(inputStream&) = 0;
			virtual void serialOut(outputStream&) const = 0;
		};

		template <typename N,
		typename = typename std::enable_if<std::is_integral<N>::value,N>::type>
		void serialIn(N& number, inputStream& stream)
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

		template <typename N,
		typename = typename std::enable_if<std::is_integral<N>::value,N>::type>
		void serialOut(N number, outputStream& stream)
		{
			if (stream.bad() || !stream.binary()) return;

			uint8_t c[sizeof(N)];
			uint8_t chars = 0;

			for (size_t i=0; i<sizeof(N); i++)
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
