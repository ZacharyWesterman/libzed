#include "dictionary.hpp"
#include "../core/charFunctions.hpp"

namespace z
{
	namespace util
	{
		dictionary::dictionary() noexcept
		{
			readingStream = false;
		}

		dictionary::~dictionary() noexcept
		{
			clear();
		}

		void dictionary::clear() noexcept
		{
			for (int i=0; i<wordList.length(); i++)
			{
				delete wordList[i];
			}
			wordList.clear();
		}

		int dictionary::read(core::inputStream& stream, const core::timeout& time) noexcept
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

		bool dictionary::isWord(const core::string<>& name) const noexcept
		{
			if (!wordList.length()) return false;

			word* check = new word(name);
			auto result = wordList.find(check);

			delete check;
			return result >= 0;
		}

		word dictionary::getWord(const core::string<>& name) const noexcept
		{
			word* check = new word(name);
			auto index = wordList.find(check);
			delete check;

			if (index >= 0)
				return word(*(wordList[index]));
			else
				return word();
		}

		int dictionary::wordCount() const noexcept
		{
			return wordList.length();
		}

		void dictionary::setWord(const word& newWord) noexcept
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

		const core::string<>& dictionary::language() const noexcept
		{
			return lang;
		}

		void dictionary::setLanguage(const core::string<>& newLang) noexcept
		{
			lang = newLang;
		}

		size_t dictionary::size() const noexcept
		{
			size_t total = wordList.size();

			for (int i=0; i<wordList.length(); i++)
				total += wordList[i]->size();

			return total;
		}

		dictRange dictionary::range() const noexcept
		{
			dictRange wordRange;

			wordRange.left = 0;
			wordRange.right = wordList.length() - 1;
			wordRange.charPos = 0;
			wordRange.isWord = false;
			wordRange.exhausted = !wordList.length();

			return wordRange;
		}

		bool dictionary::narrow(dictRange* wordRange, uint32_t nextChar) const noexcept
		{
			if (wordRange->exhausted || !wordRange) return false;
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
				wordRange->isWord = (wordRange->charPos == (wordList[wordRange->left]->get().length()));
			}

			return !(wordRange->exhausted);
		}
	}
}
