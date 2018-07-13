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

		// template <typename N, typename T,
		// typename = typename std::enable_if<std::is_arithmetic<T>::value,T>::type>
		// void serialIn(N* number, inputStream<T>* stream)
		// {
		// 	byte c[sizeof(N)];
		// 	for (int i=0; i<sizeof(N); i++) c[i] = stream->getByte();
		//
		// 	(*number) = *((N*)c);
		// }

		// template <typename N, typename T,
		// typename = typename std::enable_if<std::is_arithmetic<T>::value,T>::type>
		// void serialOut(const N number, outputStream<T>* stream)
		// {
		// 	byte* c = (byte*)&number;
		// 	for (int i=0; i<sizeof(N); i++) stream->putByte(c[i]);
		// }

		// template <typename CHAR, typename T>
		// void serialIn(core::string<CHAR>* data, inputStream<T>* stream)
		// {
		// 	Int len;
		// 	serialIn(&len, stream);
		//
		// 	(*data) = stream->read(len);
		// }
		//
		// template <typename CHAR, typename T>
		// void serialOut(const core::string<CHAR>& data, outputStream<T>* stream)
		// {
		// 	Int len = data.length();
		// 	serialOut(len, stream);
		//
		// 	stream->write(data);
		// }


	}
}
