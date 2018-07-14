#pragma once
#ifndef FILE_STREAM_H_INCLUDED
#define FILE_STREAM_H_INCLUDED

#include <fstream>
#include <stdlib.h>
#include <cstdint>

#include <z/core/stream.h>

namespace z
{
    namespace file
    {
		/**
		 * \brief A class for file input streams.
		 *
		 * This class provides an implementation of core::inputStream for files.
		 */
        class inputStream : public core::inputStream
        {
        private:
            std::ifstream filestream;

        public:
            inputStream(const core::string<utf8>&);

            uint8_t get();
			uint32_t getChar(encoding format = ascii);

			bool empty();
			bool good();
			bool bad();
			bool binary();

            void seek(size_t index);
            size_t tell();
			size_t end();
        };

		/**
		 * \brief Constructor
		 *
		 * As this is a file stream, it must be constructed with a file name.
		 *
		 * \param fileName a string containing the name of the file to read from.
		 */
        inputStream::inputStream(const core::string<utf8>& fileName)
        {
            filestream.open((const char*)fileName.cstring(), std::ios::in);
        }

        uint8_t inputStream::get()
        {
			return filestream.get();
        }

		uint32_t inputStream::getChar(encoding format)
		{
			uint32_t result;

			switch (format)
			{
			case utf16:
				result = filestream.get();
				result = (result << 8) + filestream.get();
				break;

			case utf32:
				result = filestream.get();
				result = (result << 8) + filestream.get();
				result = (result << 8) + filestream.get();
				result = (result << 8) + filestream.get();
				break;

			default:
				result = filestream.get();
			}

			return result;
		}

		void inputStream::seek(size_t index)
		{
			filestream.seekg(index, filestream.beg);
		}

		size_t inputStream::tell()
		{
			return filestream.tellg();
		}

        bool inputStream::empty()
        {
            return filestream.eof();
        }

		bool inputStream::good()
		{
			return filestream.good() && filestream.is_open();
		}

		bool inputStream::bad()
		{
			return filestream.bad() || !filestream.is_open();
		}

		bool inputStream::binary()
		{
			return true;
		}

		size_t inputStream::end()
		{
			return filestream.end - filestream.beg;
		}

		/**
		 * \brief A class for file output streams.
		 *
		 * This class provides an implementation of core::outputStream for files.
		 */
        class outputStream : public core::outputStream
        {
        private:
            std::ofstream filestream;

        public:
            outputStream(const core::string<utf8>&, bool append = false);

            void put(uint8_t ch);
			void put(uint8_t* str, size_t count, encoding format = ascii);

			bool empty();
			bool good();
			bool bad();
			bool binary();

			void seek(size_t);
			size_t tell();
			size_t end();
        };

		/**
		 * \brief Constructor
		 *
		 * As this is a file stream, it must be constructed with a file name.
		 *
		 * \param fileName a string containing the name of the file to read from.
		 * \param append set to \b true to append to the file, \b false to overwrite.
		 */
        outputStream::outputStream(const core::string<utf8>& fileName, bool append)
        {
            if (append)
                filestream.open((const char*)fileName.cstring(), std::ios::app | std::ios::out);
            else
                filestream.open((const char*)fileName.cstring(), std::ios::out);
        }

		void outputStream::put(uint8_t ch)
		{
			filestream.put(ch);
		}

		void outputStream::put(uint8_t* str, size_t count, encoding format)
		{
			size_t datact;
			switch (format)
			{
			case utf16:
				datact = count << 1;
				break;

			case utf32:
				datact = count << 2;
				break;

			default:
				datact = count;
			}

			for (size_t i=0; i<datact; i++)
				filestream.put(str[i]);
		}

		void outputStream::seek(size_t index)
		{
			filestream.seekp(index, filestream.beg);
		}

		size_t outputStream::tell()
		{
			return filestream.tellp();
		}

        bool outputStream::empty()
        {
            return filestream.eof();
        }

		bool outputStream::good()
		{
			return filestream.good() && filestream.is_open();
		}

		bool outputStream::bad()
		{
			return filestream.bad() || !filestream.is_open();
		}

		bool outputStream::binary()
		{
			return true;
		}

		size_t outputStream::end()
		{
			return filestream.end - filestream.beg;
		}
    }
}

#endif // STREAM_H_INCLUDED
