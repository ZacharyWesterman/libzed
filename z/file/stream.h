#pragma once
#ifndef FILE_STREAM_H_INCLUDED
#define FILE_STREAM_H_INCLUDED

#include <fstream>

#include <z/core/stream.h>
#include "shorten.h"

#include <z/int.h>

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
            core::string<CHAR> get(Int);
            core::string<CHAR> read(CHAR delim = 0);
            void unget();

            void seek(Int);
            Int tell();

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
        core::string<CHAR> inputStream<CHAR>::get(Int count)
        {
            core::string<CHAR> result;
            for (Int i=0; i<count; i++)
                result += filestream.get();

            return result;
        }

		template <typename CHAR>
		void inputStream<CHAR>::unget()
		{
			filestream.unget();
		}

		template <typename CHAR>
		void inputStream<CHAR>::seek(Int index)
		{
			filestream.seekg(index, filestream.beg);
		}

		template <typename CHAR>
		Int inputStream<CHAR>::tell()
		{
			return filestream.tellg();
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

			void seek(Int);
			Int tell();

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
		void outputStream<CHAR>::seek(Int index)
		{
			filestream.seekp(index, filestream.beg);
		}

		template <typename CHAR>
		Int outputStream<CHAR>::tell()
		{
			return filestream.tellp();
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
