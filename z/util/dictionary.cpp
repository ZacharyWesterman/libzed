#include "dictionary.h"

#include <z/file/exists.h>
#include <z/file/inputStream.h>

#include <z/system/console.h>

#include <z/core/timer.h>

#include <iostream>

namespace z
{
	namespace util
	{
		dictionary::~dictionary()
		{
			clear();
		}

		void dictionary::clear()
		{
			for (size_t i=0; i<wordList.length(); i++)
			{
				delete wordList[i];
			}
			wordList.clear();
		}

		bool dictionary::read(const core::string<utf8>& fileName)
		{
			clear();

			file::inputStream stream (fileName);
			const encoding format = stream.format();

			if (stream.bad()) return false;

			union
			{
				core::string<ascii>* asc;
				core::string<utf8>* ut8;
				core::string<utf16>* ut16;
				core::string<utf32>* ut32;
			} str;

			if (format == ascii)
				str.asc = new core::string<ascii>;
			else if (format == utf8)
				str.ut8 = new core::string<utf8>;
			else if (format == utf16)
				str.ut16 = new core::string<utf16>;
			else
				str.ut32 = new core::string<utf32>;

			word* result;
			if (format == ascii)
			{
				str.asc->read(stream);
				result = new word(*str.asc);
			}
			else if (format == utf8)
			{
				str.ut8->read(stream);
				result = new word(*str.ut8);
			}
			else if (format == utf16)
			{
				str.ut16->read(stream);
				result = new word(*str.ut16);
			}
			else
			{
				str.ut32->read(stream);
				result = new word(*str.ut32);
			}

			while (!stream.empty())
			{
				wordList.add(result);

				if (format == ascii)
				{
					str.asc->read(stream);
					result = new word(*str.asc);
				}
				else if (format == utf8)
				{
					str.ut8->read(stream);
					result = new word(*str.ut8);
				}
				else if (format == utf16)
				{
					str.ut16->read(stream);
					result = new word(*str.ut16);
				}
				else
				{
					str.ut32->read(stream);
					result = new word(*str.ut32);
				}
			}

			delete result;

			if (format == ascii)
				delete str.asc;
			else if (format == utf8)
				delete str.ut8;
			else if (format == utf16)
				delete str.ut16;
			else
				delete str.ut32;

			return true;
		}

		bool dictionary::isWord(const core::string<Z_DICT_FORMAT>& name) const
		{
			if (!wordList.length()) return false;

			word* check = new word(name);
			auto result = wordList.find(check);

			delete check;
			return result >= 0;
		}

		word dictionary::getWord(const core::string<Z_DICT_FORMAT>& name) const
		{
			word* check = new word(name);
			auto index = wordList.find(check);
			delete check;

			if (index >= 0)
				return word(*(wordList[index]));
			else
				return word();
		}

		size_t dictionary::wordCount() const
		{
			return wordList.length();
		}

		void dictionary::setWord(const word& newWord)
		{
			word* item = new word(newWord);

			auto index = wordList.find(item);

			if (index >= 0)
			{
				delete wordList[index];
				wordList[index] = item;
			}
			else
			{
				wordList.add(item);
			}
		}

		const core::string<Z_DICT_FORMAT>& dictionary::language() const
		{
			return lang;
		}

		void dictionary::setLanguage(const core::string<Z_DICT_FORMAT>& newLang)
		{
			lang = newLang;
		}

		size_t dictionary::size() const
		{
			size_t total = wordList.size();

			for (size_t i=0; i<wordList.length(); i++)
				total += wordList[i]->size();

			return total;
		}

		void dictionary::serialIn(core::inputStream& stream)
		{
			if (stream.bad()) return;
			clear();

			// the dict file was corrupted / did not finish writing.
			if (stream.get()) return;

			lang.serialIn(stream);

			size_t length = 0;
			core::serialIn(length, stream);

			word temp;
			for (size_t i=0; i<length; i++)
			{
				if (stream.empty()) return;
				temp.serialIn(stream);
				wordList.append(new word(temp));
			}
		}

		void dictionary::serialOut(core::outputStream& stream) const
		{
			if (stream.bad()) return;

			auto start = stream.tell();

			stream.put(0xFF); //indicate we're not done writing

			lang.serialOut(stream);

			size_t length = wordList.length();
			core::serialOut(length, stream);

			for (size_t i=0; i<wordList.length(); i++)
			{
				wordList[i]->serialOut(stream);
			}

			auto stop = stream.tell();

			stream.seek(start);
			stream.put(0); //we finished writing the dictionary.

			stream.seek(stop);
		}
	}
}
