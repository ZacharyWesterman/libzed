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
        class inputStream : public core::inputStream<CHAR>
        {
        private:
            std::basic_ifstream<CHAR> filestream;

        public:
            inputStream(const core::string<char>&);

            CHAR get();
            core::string<CHAR> read(CHAR delim = 0);

            bool empty();
        };

        template <typename CHAR>
        inputStream<CHAR>::inputStream(const core::string<char>& fileName)
        {
            filestream.open(fileName.str(), std::ios::in);
        }

        template <typename CHAR>
        CHAR inputStream<CHAR>::get()
        {
            return filestream.get();
        }

        template <typename CHAR>
        core::string<CHAR> inputStream<CHAR>::read(CHAR delim)
        {
            CHAR tmp[256];

            if (delim)
                filestream.getline(tmp, 256, delim);
            else
                filestream >> tmp;

            return core::string<CHAR>(tmp);
        }

        template <typename CHAR>
        bool inputStream<CHAR>::empty()
        {
            return (filestream.bad() || filestream.eof() || filestream.fail());
        }



        template <typename CHAR>
        class outputStream : public core::outputStream<CHAR>
        {
        private:
            std::basic_ofstream<CHAR> filestream;

        public:
            outputStream(const core::string<char>&, bool append = false);

            void put(CHAR);
            void write(const core::string<CHAR>&);

            bool empty();
        };

        template <typename CHAR>
        outputStream<CHAR>::outputStream(const core::string<char>& fileName, bool append)
        {
            if (append)
                filestream.open(fileName.str(), std::ios::app | std::ios::out);
            else
                filestream.open(fileName.str(), std::ios::out);
        }

        template <typename CHAR>
        void outputStream<CHAR>::put(CHAR input)
        {
            filestream.put(input);
        }

        template <typename CHAR>
        void outputStream<CHAR>::write(const core::string<CHAR>& input)
        {
            filestream << input.str();
        }

        template <typename CHAR>
        bool outputStream<CHAR>::empty()
        {
            return (filestream.bad() || filestream.eof() || filestream.fail());
        }
    }
}

#endif // STREAM_H_INCLUDED
