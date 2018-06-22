#pragma once
#ifndef FILE_STREAM_H_INCLUDED
#define FILE_STREAM_H_INCLUDED

#include <fstream>
#include <stdlib.h>
#include <cstdint>

#include <z/core/charFunctions.h>
#include <z/core/stream.h>
#include "shorten.h"

#include <z/int.h>

#include "encoding.h"

namespace z
{
    namespace file
    {
		/**
		 * \brief A template class for file input streams.
		 *
		 * This class provides an implementation of core::inputStream for files.
		 */
        template <typename CHAR>
        class inputStream : public core::inputStream<CHAR>
        {
        private:
            std::ifstream filestream;
			encoding format;

			CHAR readCharASCII();
			CHAR readCharUTF_8();
			CHAR readCharUTF16();
			CHAR readCharUTF32();

			CHAR readChar();

        public:
            inputStream(const core::string<char>&);

            CHAR get();
			byte getByte();
            core::string<CHAR> get(Int);
            core::string<CHAR> read(CHAR delim = 0);
            void unget();

			bool empty();

            void seek(Int);
            Int tell();

			Int end();

			void setEncoding(encoding);
        };

		/**
		 * \brief Constructor
		 *
		 * As this is a file stream, it must be constructed with a file name.
		 *
		 * \param fileName a string containing the name of the file to read from.
		 */
        template <typename CHAR>
        inputStream<CHAR>::inputStream(const core::string<char>& fileName)
        {
            filestream.open(fileName.str(), std::ios::in);

			format = UTF_8;
        }

		template <>
        inputStream<char>::inputStream(const core::string<char>& fileName)
        {
            filestream.open(fileName.str(), std::ios::in);

			format = ASCII;
        }

        template <typename CHAR>
        CHAR inputStream<CHAR>::get()
        {
			return readChar();
        }

		template <typename CHAR>
		byte inputStream<CHAR>::getByte()
		{
			return filestream.get();
		}

        template <typename CHAR>
        core::string<CHAR> inputStream<CHAR>::get(Int count)
        {
            core::string<CHAR> result;
            for (Int i=0; i<count; i++)
        	{
				CHAR chr = readChar();
				if (this->empty()) return result;

				result += chr;
			}

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
			core::string<CHAR> result;

			bool done = false;
			bool readStart = false;

			while (!done)
			{
				CHAR chr = readChar();
				if (this->empty()) return result;


				if (delim)
				{
					if (chr != delim)
					{
						readStart = true;
						result += chr;
					}
					else if (readStart)
						done = true;
				}
				else
				{
					if (!core::isWhiteSpace(chr))
					{
						readStart = true;
						result += chr;
					}
					else if (readStart)
						done = true;
				}
			}


			return result;
        }

		template <typename CHAR>
		CHAR inputStream<CHAR>::readChar()
		{
			switch (format)
			{
			case ASCII:
				return readCharASCII();
			case UTF_8:
				return readCharUTF_8();
			case UTF16:
				return readCharUTF16();
			case UTF32:
				return readCharUTF32();
			default:
				return readCharUTF_8();
			}
		}

        template <typename CHAR>
        bool inputStream<CHAR>::empty()
        {
            return (filestream.bad() || filestream.eof() || filestream.fail());
        }

		template <typename CHAR>
		Int inputStream<CHAR>::end()
		{
			return filestream.end - filestream.beg;
		}

		template <typename CHAR>
		void inputStream<CHAR>::setEncoding(encoding enc)
		{
			format = enc;
		}

		template <typename CHAR>
		CHAR inputStream<CHAR>::readCharASCII()
		{
			return filestream.get();
		}

		template <typename CHAR>
		CHAR inputStream<CHAR>::readCharUTF_8()
		{
			uint32_t chr = filestream.get();
			uint32_t result;

			if (chr < 0x80)//1 char, ascii
			{
				return chr;
			}
			else if (chr < 0xC0)//invalid utf-8, just return the char.
			{
				return chr;
			}
			else if (chr < 0xE0)//2 chars
			{
				char chr2 = filestream.get();

				result = ((chr & 0x1F) << 6) + (chr2 & 0x3F);
			}
			else if (chr < 0xF0)//3 chars
			{
				char c[2];
				for (int i=0; i<2; i++) c[i] = filestream.get();

				result = ((chr & 0x0F) << 12) + ((c[0] & 0x3F) << 6) + (c[1] & 0x3F);
			}
			else if (chr < 0xF8) //4 chars
			{
				char c[3];
				for (int i=0; i<3; i++) c[i] = filestream.get();

				result = ((chr & 0x07) << 18) + ((c[0] & 0x3F) << 12) + ((c[1] & 0x3F) << 6) + (c[2] & 0x3F);
			}
			else//invalid utf-8, just return the char.
			{
				return chr;
			}

			return result;
		}

		template <typename CHAR>
		CHAR inputStream<CHAR>::readCharUTF16()
		{
			char c[2];
			c[0] = filestream.get();
			c[1] = filestream.get();

			int16_t value = *((int16_t*)c);

			return (CHAR)value;
		}

		template <typename CHAR>
		CHAR inputStream<CHAR>::readCharUTF32()
		{
			char c[4];
			for (int i=0; i<4; i++) c[i] = filestream.get();

			int32_t value = *((int32_t*)c);

			return (CHAR)value;
		}

		/**
		 * \brief A template class for file output streams.
		 *
		 * This class provides an implementation of core::outputStream for files.
		 */
        template <typename CHAR>
        class outputStream : public core::outputStream<CHAR>
        {
        private:
            std::ofstream filestream;
			encoding format;

			void writeCharASCII(CHAR);
			void writeCharUTF_8(CHAR);
			void writeCharUTF16(CHAR);
			void writeCharUTF32(CHAR);

			void writeChar(CHAR);

        public:
            outputStream(const core::string<char>&, bool append = false, encoding fileFormat = UTF_8);

            void put(CHAR);
			void putByte(byte);
            void write(const core::string<CHAR>&);

			void seek(Int);
			Int tell();

            bool empty();
			Int end();

			void setEncoding(encoding);
        };

		/**
		 * \brief Constructor
		 *
		 * As this is a file stream, it must be constructed with a file name.
		 *
		 * \param fileName a string containing the name of the file to read from.
		 * \param append set to \b true to append to the file, \b false to overwrite.
		 */
        template <typename CHAR>
        outputStream<CHAR>::outputStream(const core::string<char>& fileName, bool append, encoding fileFormat)
        {
            if (append)
                filestream.open(fileName.str(), std::ios::app | std::ios::out);
            else
                filestream.open(fileName.str(), std::ios::out);

			format = fileFormat;
        }

        template <typename CHAR>
        void outputStream<CHAR>::put(CHAR input)
        {
			writeChar(input);
        }

		template <typename CHAR>
		void outputStream<CHAR>::putByte(byte input)
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
			for (Int i=0; i<input.length(); i++)
			{
				writeChar(input.at(i));
			}
        }

		template <typename CHAR>
		void outputStream<CHAR>::writeChar(CHAR chr)
		{
			switch (format)
			{
			case ASCII:
				writeCharASCII(chr);
				break;
			case UTF_8:
				writeCharUTF_8(chr);
				break;
			case UTF16:
				writeCharUTF16(chr);
				break;
			case UTF32:
				writeCharUTF32(chr);
				break;
			default:
				writeCharUTF_8(chr);
			}
		}

        template <typename CHAR>
        bool outputStream<CHAR>::empty()
        {
            return (filestream.bad() || filestream.eof() || filestream.fail());
        }

		template <typename CHAR>
		Int outputStream<CHAR>::end()
		{
			return filestream.end - filestream.beg;
		}

		template <typename CHAR>
		void outputStream<CHAR>::setEncoding(encoding enc)
		{
			format = enc;
		}

		template <typename CHAR>
		void outputStream<CHAR>::writeCharASCII(CHAR chr)
		{
			if (chr > 0xFF)
			{
				filestream.put('?');
			}
			else
			{
				filestream.put((char)chr);
			}
		}

		template <typename CHAR>
		void outputStream<CHAR>::writeCharUTF_8(CHAR chr)
		{
			char c[4];
			int bufsiz;

			if (chr < 0x80)
			{
				c[0] = chr;
				bufsiz = 1;
			}
			else if (chr < 0x0800)
			{
				c[0] = ((chr >> 6) & 0x1F) + 0xC0;
				c[1] = (chr & 0x3F) + 0x80;

				bufsiz = 2;
			}
			else if (chr < 0xFFFF)
			{
				c[0] = ((chr >> 12) & 0x0F) + 0xE0;
				c[1] = ((chr >> 6) & 0x3F) + 0x80;
				c[2] = (chr & 0x3F) + 0x80;

				bufsiz = 3;
			}
			else if (chr < 0x110000)
			{
				c[0] = ((chr >> 18) & 0x07) + 0xF0;
				c[1] = ((chr >> 12) & 0x3F) + 0x80;
				c[2] = ((chr >> 6) & 0x3F) + 0x80;
				c[3] = (chr & 0x3F) + 0x80;

				bufsiz = 4;
			}
			else
			{
				c[0] = '?';
				bufsiz = 1;
			}

			for (int i=0; i<bufsiz; i++) filestream.put(c[i]);
		}

		template <typename CHAR>
		void outputStream<CHAR>::writeCharUTF16(CHAR chr)
		{
			if (chr > 0xFFFF)
			{
				filestream.put('?');
			}
			else
			{
				int16_t num = chr;
				char* buf = (char*)&num;

				filestream.put(buf[0]);
				filestream.put(buf[1]);
			}
		}

		template <typename CHAR>
		void outputStream<CHAR>::writeCharUTF32(CHAR chr)
		{
			if (chr > 0xFFFFFFFF)
			{
				filestream.put('?');
			}
			else
			{
				int32_t num = chr;
				char* buf = (char*)&num;

				for (int i=0; i<4; i++) filestream.put(buf[i]);
			}
		}
    }
}

#endif // STREAM_H_INCLUDED
