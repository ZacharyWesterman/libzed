#pragma once
#ifndef BINARYSTREAM_H_INCLUDED
#define BINARYSTREAM_H_INCLUDED

#include "array.h"
#include "stream.h"

namespace z
{
	namespace core
	{
		template <encoding E>
		class binaryStream : public stream<E>
		{
		private:
			array<uint8_t> data;
			size_t index;

		public:
			binaryStream();

			uint32_t get();
			uint8_t getByte();
			string<E> get(size_t);

			void unget();

			void put(uint32_t);
			void putByte(uint8_t);

			string<E> read(uint32_t delim = 0);
			void write(const string<E>&);

			bool empty();
			void seek(size_t);
			size_t tell();

			size_t end();
		};


		template <encoding E>
		binaryStream<E>::binaryStream() : index(0) {}

		template <encoding E>
		uint32_t binaryStream<E>::get()
		{
			if (data.isValid(index))
				return data.at(index++);
			else
				return 0;
		}

		template <encoding E>
		uint8_t binaryStream<E>::getByte()
		{
			
		}

		template <typename CHAR>
		string<CHAR> binaryStream<CHAR>::get(Int count)
		{
			string<CHAR> result;

			Int i = 0;
			while ((i < count) && data.isValid(index+i))
				result += data.at(index+i);

			index += count;

			return result;
		}

		template <typename CHAR>
		void binaryStream<CHAR>::unget()
		{
			if (index) index--;
		}

		template <typename CHAR>
		void binaryStream<CHAR>::put(CHAR c)
		{
			data.add(c);
		}

		template <typename CHAR>
		string<CHAR> binaryStream<CHAR>::read(CHAR delim)
		{
			string<CHAR> result;

			while (data.isValid(index) && (data.at(index) == delim))
				index++;

			while (data.isValid(index) && (data.at(index) != delim))
				result += data.at(index++);

			return result;
		}

		template <typename CHAR>
		void binaryStream<CHAR>::write(const string<CHAR>& input)
		{
			for (Int i=0; i<input.length(); i++)
				data.add(input[i]);
		}

		template <typename CHAR>
		bool binaryStream<CHAR>::empty()
		{
			return index >= data.length();
		}

		template <typename CHAR>
		void binaryStream<CHAR>::seek(Int position)
		{
			if ((position < 0) || (position >= data.length()))
                index = data.length();
            else
                index = position;
		}

		template <typename CHAR>
		Int binaryStream<CHAR>::tell()
		{
			return index;
		}

		template <typename CHAR>
		Int binaryStream<CHAR>::end()
		{
			return data.length();
		}
	}
}

#endif //BINARYSTREAM_H_INCLUDED
