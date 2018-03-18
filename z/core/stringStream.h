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
        template <typename CHAR>
        class stringStream : public stream<CHAR>
        {
        private:
            string<CHAR> data;
            Int index;

        public:
            stringStream(const string<CHAR>&);

            CHAR get();
            void unget();
            void put(CHAR);
            CHAR unput();

            string<CHAR> read(CHAR delim = 0);
            void write(const string<CHAR>&);

            bool empty();
            void seek(int);
            int tell() const;
        };

        template <typename CHAR>
        stringStream<CHAR>::stringStream(const string<CHAR>& input) : data(input), index(0) {}

        template <typename CHAR>
        CHAR stringStream<CHAR>::get()
        {
            CHAR c = data[index];
            index++;

            return c;
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
        CHAR stringStream<CHAR>::unput()
        {
            Int pos = data.length()-1;

            CHAR c = data[pos];
            data.remove(pos, pos);

            return c;
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
        void stringStream<CHAR>::seek(int position)
        {
            if ((position < 0) || (position >= data.length()))
                index = data.length();
            else
                index = position;
        }

        template <typename CHAR>
        int stringStream<CHAR>::tell() const
        {
            return index;
        }
    }
}

#endif // STRINGSTREAM_H_INCLUDED