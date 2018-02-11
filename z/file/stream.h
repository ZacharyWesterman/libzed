#pragma once
#ifndef FILE_STREAM_H_INCLUDED
#define FILE_STREAM_H_INCLUDED

#include <fstream>

#include <z/core/stream.h>
#include "shorten.h"

namespace z
{
    namespace file
    {
        template <typename CHAR>
        class stream : public core::stream<CHAR>
        {
        private:
            std::basic_fstream<CHAR> filestream;
            bool is_done;

        public:
            stream(const core::string<char>&, bool append = false);

            CHAR get();
            void put(CHAR);

            void write(const core::string<CHAR>&);
            core::string<CHAR> read(CHAR delim = ' ');

            bool empty();
        };

        template <typename CHAR>
        stream<CHAR>::stream(const core::string<char>& fileName, bool append)
        {
            is_done = false;

            if (append)
                filestream.open(fileName.str(), std::ios::app);
            else
                filestream.open(fileName.str());
        }

        template <typename CHAR>
        CHAR stream<CHAR>::get()
        {
            return filestream.get();
        }

        template <typename CHAR>
        void stream<CHAR>::put(CHAR input)
        {
            filestream.put(input);
        }

        template <typename CHAR>
        core::string<CHAR> stream<CHAR>::read(CHAR delim)
        {
            CHAR tmp[256];
            filestream.read(tmp, 256);

            return core::string<CHAR>(tmp);
        }

        template <typename CHAR>
        void stream<CHAR>::write(const core::string<CHAR>& input)
        {
            filestream.write(input.str(), input.length());
        }

        template <typename CHAR>
        bool stream<CHAR>::empty()
        {
            return (filestream.bad() || filestream.eof());
        }
    }
}

#endif // STREAM_H_INCLUDED
