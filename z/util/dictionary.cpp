#include "dictionary.hpp"
#include <z/core/charFunctions.hpp>

#include <iostream>

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

			core::string<> name;

			name.read(stream);
			while (!(stream.empty() || time.timedOut()))
			{
				wordList.add(new word(name));

				name.read(stream);
			}

			return stream.empty();
		}

		bool dictionary::isWord(const core::string<>& name) const
		{
			if (!wordList.length()) return false;

			word* check = new word(name);
			auto result = wordList.find(check);

			delete check;
			return result >= 0;
		}

		word dictionary::getWord(const core::string<>& name) const
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

		const core::string<>& dictionary::language() const
		{
			return lang;
		}

		void dictionary::setLanguage(const core::string<>& newLang)
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
			stream.flush();

			lang.serialOut(stream);

			size_t length = wordList.length();
			core::serialOut(length, stream);

			for (size_t i=0; i<wordList.length(); i++)
			{
				wordList[i]->serialOut(stream);
			}

			auto stop = stream.tell();

			stream.seek(start);

			stream.flush();
			stream.put(0); //we finished writing the dictionary.
			stream.flush();

			stream.seek(stop);
		}

		dictRange dictionary::range() const
		{
			dictRange wordRange;

			wordRange.left = 0;
			wordRange.right = wordList.length() - 1;
			wordRange.charPos = 0;
			wordRange.isWord = false;
			wordRange.exhausted = !wordList.length();

			return wordRange;
		}

		bool dictionary::narrow(dictRange* wordRange, uint32_t nextChar) const
		{
			if (wordRange->exhausted) return false;
			wordRange->isWord = false;

			nextChar = z::core::toUpper(nextChar);

			//get furthest left
			int left = wordRange->left;
			int right = wordRange->right;
			while (left < right)
			{
				int center = (left + right) >> 1;
				auto thisChar = z::core::toUpper(wordList[center]->get()[wordRange->charPos]);

				if (thisChar < nextChar)
					left = center + 1;
				else
					right = center - 1;
			}
			if (z::core::toUpper(wordList[left]->get()[wordRange->charPos]) < nextChar) ++left;

			//If we've skipped past the last successful match, we're done.
			if (wordRange->charPos)
			{
				auto oldLast = z::core::toUpper(wordList[wordRange->left]->get()[wordRange->charPos - 1]);
				auto newLast = z::core::toUpper(wordList[left]->get()[wordRange->charPos - 1]);

				if (oldLast != newLast)
				{
					wordRange->exhausted = true;
					return false;
				}
			}
			wordRange->left = left;

			//get furthest right
			left = wordRange->left;
			right = wordRange->right;
			while (left < right)
			{
				int center = (left + right) >> 1;
				auto thisChar = z::core::toUpper(wordList[center]->get()[wordRange->charPos]);

				if (thisChar > nextChar)
					right = center - 1;
				else
					left = center + 1;
			}
			if (z::core::toUpper(wordList[right]->get()[wordRange->charPos]) < nextChar) --right;
			wordRange->right = right;

			if (z::core::toUpper(wordList[wordRange->left]->get()[wordRange->charPos]) != nextChar)
			{
				wordRange->exhausted = true;
			}
			else
			{
				++(wordRange->charPos);
				wordRange->isWord = ((size_t)wordRange->charPos == (wordList[wordRange->left]->get().length()));
				// if (wordRange->isWord)
				// 	std::cout << zpath(wordList[wordRange->left]->get()).upper().cstring() << " > " << zpath(wordList[wordRange->left]->get()).cstring() << " : " << zpath(wordList[wordRange->right]->get()).cstring() << std::endl;
			}

			return !(wordRange->exhausted);
		}
	}
}
