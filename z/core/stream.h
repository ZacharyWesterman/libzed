#pragma once
#ifndef STREAM_H_INCLUDED
#define STREAM_H_INCLUDED

#include <type_traits>

#include "string.h"

#ifndef null
    #define null 0
#endif // null

static const int ETX = 3; //end of text
static const int SI  = 15; //shift-in
static const int SO  = 14; //shift-out

namespace z
{
    namespace core
    {
        /**
         * \brief A template class for character streams.
         *
         *
         */
        template <typename CHAR>
        class stream
        {
        protected:


        public:
            virtual ~stream() {}

            virtual void putChar(CHAR) = 0;

            virtual CHAR getChar() = 0;

            ///Operator for input to stream
            virtual stream& operator<<(string<CHAR>&) = 0;

            ///Operator for output from stream
            virtual stream& operator>>(string<CHAR>&) = 0;

            virtual bool eof() = 0;
        };

    }
}

#endif // STREAM_H_INCLUDED
