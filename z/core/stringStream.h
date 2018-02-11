#pragma once
#ifndef STRINGSTREAM_H_INCLUDED
#define STRINGSTREAM_H_INCLUDED

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

        public:
            stringStream(const string<CHAR>&);

            CHAR get();
            void put(CHAR);

            string<CHAR> read(CHAR delim = 0);
            void write(const string<CHAR>&);

            bool empty();
        };

        template <typename CHAR>
        stringStream<CHAR>::stringStream(const string<CHAR>& input) : data(input) {}

        template <typename CHAR>
        CHAR stringStream<CHAR>::get()
        {
            CHAR c = data[0];
            data.remove(0,0);

            return c;
        }

        template <typename CHAR>
        void stringStream<CHAR>::put(CHAR c)
        {
            data += c;
        }

        template <typename CHAR>
        string<CHAR> stringStream<CHAR>::read(CHAR delim)
        {
            int begIndex = 0;

            if (delim)
            {
                while ((begIndex < data.length()) && (data[begIndex] == delim))
                    begIndex++;
            }
            else
            {
                while ((begIndex < data.length()) && !isWhiteSpace(data[begIndex]))
                    begIndex++;
            }


            int endIndex;

            if (delim)
            {
                endIndex = data.findAfter(delim, begIndex);
                if (endIndex == -1)
                    endIndex = data.length();
                endIndex--;
            }
            else
            {
                endIndex = begIndex+1;
                while ((endIndex < data.length()) && !isWhiteSpace(data[endIndex]))
                    endIndex++;
            }

            string<CHAR> result = data.substr(begIndex+1, endIndex-1);
            data.remove(0, endIndex-1);

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
            return (data.length() <= 0);
        }
    }
}

#endif // STRINGSTREAM_H_INCLUDED
