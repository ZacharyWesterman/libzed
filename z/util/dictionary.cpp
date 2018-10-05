#include "dictionary.h"

namespace z
{
	namespace util
	{
		dictionary::dictionary()
		{
			readingStream = false;
		}

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

		word* dictionary::readWordFromStream(core::inputStream& stream)
		{
			word* result;

			if (streamFormat == ascii)
			{
				core::string<ascii> str;
				str.read(stream);
				result = new word(str);
			}
			else if (streamFormat == utf8)
			{
				core::string<utf8> str;
				str.read(stream);
				result = new word(str);
			}
			else if (streamFormat == utf16)
			{
				core::string<utf16> str;
				str.read(stream);
				result = new word(str);
			}
			else
			{
				core::string<utf32> str;
				str.read(stream);
				result = new word(str);
			}

			return result;
		}

		int dictionary::read(core::inputStream& stream, const core::timeout& time)
		{
			if (stream.bad()) return -1;
			if (stream.empty()) return 1;

			if (!readingStream)
			{
				clear();
				streamFormat = stream.format();
				readingStream = true;
			}

			word* newWord = readWordFromStream(stream);

			while (!(stream.empty() || time.timedOut()))
			{
				wordList.add(newWord);

				newWord = readWordFromStream(stream);
			}

			if (stream.empty())
			{
				delete newWord;
				return 1;
			}
			else
			{
				wordList.add(newWord);
				return 0;
			}
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
