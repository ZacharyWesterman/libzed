#include "dictionary.hpp"
#include "../core/charFunctions.hpp"

namespace z
{
	namespace util
	{
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

		int dictionary::read(std::ifstream& stream, const core::timeout& time, bool assumePresorted) noexcept
		{
			if (stream.bad()) return -1;
			if (stream.eof()) return 1;

			if (!readingStream)
			{
				clear();
				readingStream = true;
			}

			while (!(stream.eof() || time.timedOut()))
			{
				std::string w;
				stream >> w;
				zstring* word = new zstring(w.c_str());
				if (word->length())
				{
					if (assumePresorted)
						wordList.append(word);
					else
						wordList.add(word);
				}
				else
					delete word;
			}

			return stream.eof();
		}

		bool dictionary::isWord(const zstring& word) const noexcept
		{
			auto index = wordList.find(&word);
			return index >= 0;
		}

		int dictionary::find(const zstring& word) const noexcept
		{
			return wordList.find(&word);
		}

		int dictionary::length() const noexcept
		{
			return wordList.length();
		}

		void dictionary::addWord(const zstring& word) noexcept
		{
			auto index = wordList.find(&word);
			if (index < 0) wordList.add(new zstring(word));
		}

		const zstring& dictionary::language() const noexcept
		{
			return lang;
		}

		void dictionary::setLanguage(const zstring& newLang) noexcept
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
				auto thisChar = z::core::toUpper(wordList[center]->at(wordRange->charPos));

				if (thisChar < nextChar)
					left = center + 1;
				else
					right = center - 1;
			}
			if (z::core::toUpper(wordList[left]->at(wordRange->charPos)) < nextChar) ++left;

			//If we've skipped past the last successful match, we're done.
			if (wordRange->charPos)
			{
				auto oldLast = z::core::toUpper(wordList[wordRange->left]->at(wordRange->charPos - 1));
				auto newLast = z::core::toUpper(wordList[left]->at(wordRange->charPos - 1));

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
				auto thisChar = z::core::toUpper(wordList[center]->at(wordRange->charPos));

				if (thisChar > nextChar)
					right = center - 1;
				else
					left = center + 1;
			}
			if (z::core::toUpper(wordList[right]->at(wordRange->charPos)) < nextChar) --right;
			wordRange->right = right;

			if (z::core::toUpper(wordList[wordRange->left]->at(wordRange->charPos)) != nextChar)
			{
				wordRange->exhausted = true;
			}
			else
			{
				++(wordRange->charPos);
				wordRange->isWord = (wordRange->charPos == (wordList[wordRange->left]->length()));
			}

			return !(wordRange->exhausted);
		}
	}
}
