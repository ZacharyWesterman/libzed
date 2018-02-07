#pragma once
#ifndef CORE_STREAM_H_INCLUDED
#define CORE_STREAM_H_INCLUDED

#include "string.h"

namespace z
{
    namespace core
    {
        template <typename CHAR>
        class inputStream
        {
        public:
            virtual ~inputStream() {}

            virtual CHAR get() = 0;
            virtual core::string<CHAR> read(CHAR delim = ' ') = 0;

            virtual bool empty() = 0;
        };

        template <typename CHAR>
        class outputStream
        {
        public:
            virtual ~outputStream() {}

            virtual void put(CHAR) = 0;
            virtual void write(const core::string<CHAR>&) = 0;

            virtual bool empty() = 0;
        };

        /**
         * \brief A template class for character streams.
         *
         *
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
