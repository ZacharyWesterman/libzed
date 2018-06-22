#pragma once
#ifndef STRINGSTREAM_H_INCLUDED
#define STRINGSTREAM_H_INCLUDED

#include <z/int.h>
#include "charFunctions.h"
#include "string.h"
#include "stream.h"

namespace z
{
    namespace core
    {
		/**
		 * \brief A template class for string streams.
		 *
		 * This class provides an implementation of core::stream for character strings.
		 */
        template <typename CHAR>
        class stringStream : public stream<CHAR>
        {
        private:
            string<CHAR> data;
            Int index;

        public:
            stringStream(const string<CHAR>& input = string<CHAR>());

            CHAR get();
			byte getByte();
            string<CHAR> get(Int);
            void unget();
            void put(CHAR);
			void putByte(byte);

            string<CHAR> read(CHAR delim = 0);
            void write(const string<CHAR>&);

            bool empty();
            void seek(Int);
            Int tell();

			Int end();
        };

		/**
		 * \brief Constructor
		 *
		 * \param input initial string for the stream to contain.
		 */
        template <typename CHAR>
        stringStream<CHAR>::stringStream(const string<CHAR>& input) :
			data(input), index(0) {}

        template <typename CHAR>
        CHAR stringStream<CHAR>::get()
        {
            CHAR c = data[index];
            index++;

            return c;
        }

		template <typename CHAR>
		byte stringStream<CHAR>::getByte()
		{
			return this->get();
		}

        template <typename CHAR>
        string<CHAR> stringStream<CHAR>::get(Int count)
        {
            if (count < 1) return string<CHAR>();

            string<CHAR> result = data.substr(index, index+count-1);
            index += count;

            return result;
        }

        template <typename CHAR>
        void stringStream<CHAR>::unget()
        {
            if (index)
                index--;
        }

        template <typename CHAR>
        void stringStream<CHAR>::put(CHAR c)
        {
            data += string<CHAR>(c);
        }

		template <typename CHAR>
		void stringStream<CHAR>::putByte(byte b)
		{
			this->put(b);
		}

        template <typename CHAR>
        string<CHAR> stringStream<CHAR>::read(CHAR delim)
        {
            Int begIndex = index;

            if (delim)
            {
                while ((begIndex < data.length()) && (data[begIndex] == delim))
                    begIndex++;
            }
            else
            {
                while ((begIndex < data.length()) && isWhiteSpace(data[begIndex]))
                    begIndex++;
            }


            Int endIndex;

            if (delim)
            {
                endIndex = data.findAfter(delim, begIndex);
                if (endIndex == -1)
                    endIndex = data.length();
            }
            else
            {
                endIndex = begIndex+1;
                while ((endIndex < data.length()) && !isWhiteSpace(data[endIndex]))
                    endIndex++;
            }

            string<CHAR> result = data.substr(begIndex, endIndex-1);
            index = endIndex;

            return result;
        }

        template <typename CHAR>
        void stringStream<CHAR>::write(const string<CHAR>& input)
        {
            data += input;
        }

        template <typename CHAR>
        bool stringStream<CHAR>::empty()
        {
            return (index >= data.length());
        }

        template <typename CHAR>
        void stringStream<CHAR>::seek(Int position)
        {
            if ((position < 0) || (position >= data.length()))
                index = data.length();
            else
                index = position;
        }

        template <typename CHAR>
        Int stringStream<CHAR>::tell()
        {
            return index;
        }

		template <typename CHAR>
        Int stringStream<CHAR>::end()
        {
            return data.length();
        }
    }
}

#endif // STRINGSTREAM_H_INCLUDED
