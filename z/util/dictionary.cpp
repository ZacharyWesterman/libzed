#include "dictionary.hpp"
#include "../core/charFunctions.hpp"

#include <algorithm>
#include <iostream>

namespace z {
namespace util {
dictionary::~dictionary() noexcept {
	clear();
}

void dictionary::clear() noexcept {
	for (int i = 0; i < wordList.length(); i++) {
		delete wordList[i];
	}
	wordList.clear();
}

int dictionary::read(std::istream &stream, const core::timeout &time, bool assumePresorted) noexcept {
	if (stream.fail())
		return -1;
	if (stream.eof())
		return 1;

	if (!readingStream) {
		clear();
		maxWordLen = 0;
		readingStream = true;
	}

	while (!(stream.eof() || time.timedOut())) {
		zstring *word = new zstring;
		word->read(stream);
		if (word->length()) {
			if (!caseSensitive)
				word->toLower();

			if (assumePresorted)
				wordList.append(word);
			else
				wordList.add(word);

			maxWordLen = std::max(word->length(), maxWordLen);
		} else
			delete word;
	}

	return stream.eof();
}

bool dictionary::isWord(const zstring &word) const noexcept {
	return find(word) >= 0;
}

int dictionary::find(const zstring &word) const noexcept {
	if (caseSensitive) {
		return wordList.find(&word);
	} else {
		auto newWord = word.lower();
		return wordList.find(&newWord);
	}
}

int dictionary::length() const noexcept {
	return wordList.length();
}

void dictionary::addWord(const zstring &word) noexcept {
	auto wordptr = new zstring(caseSensitive ? word : word.lower());
	maxWordLen = std::max(wordptr->length(), maxWordLen);
	if (find(wordptr) < 0)
		wordList.add(wordptr);
}

bool dictionary::isCaseSensitive() const noexcept {
	return caseSensitive;
}

void dictionary::setCaseSensitive(bool caseSensitive) noexcept {
	this->caseSensitive = caseSensitive;
}

size_t dictionary::size() const noexcept {
	size_t total = wordList.size();

	for (int i = 0; i < wordList.length(); i++)
		total += wordList[i]->size();

	return total;
}

dictRange dictionary::range() const noexcept {
	dictRange wordRange;

	wordRange.left = 0;
	wordRange.right = wordList.length() - 1;
	wordRange.charPos = 0;
	wordRange.isWord = false;
	wordRange.exhausted = !wordList.length();

	return wordRange;
}

bool dictionary::narrow(dictRange *wordRange, uint32_t nextChar) const noexcept {
	if (wordRange->exhausted || !wordRange)
		return false;
	wordRange->isWord = false;

	nextChar = z::core::toUpper(nextChar);

	// get furthest left
	int left = wordRange->left;
	int right = wordRange->right;
	while (left < right) {
		int center = (left + right) >> 1;
		auto thisChar = z::core::toUpper(wordList[center]->at(wordRange->charPos));

		if (thisChar < nextChar)
			left = center + 1;
		else
			right = center - 1;
	}
	if (z::core::toUpper(wordList[left]->at(wordRange->charPos)) < nextChar)
		++left;

	// If we've skipped past the last successful match, we're done.
	if (wordRange->charPos) {
		auto oldLast = z::core::toUpper(wordList[wordRange->left]->at(wordRange->charPos - 1));
		auto newLast = z::core::toUpper(wordList[left]->at(wordRange->charPos - 1));

		if (oldLast != newLast) {
			wordRange->exhausted = true;
			return false;
		}
	}
	wordRange->left = left;

	// get furthest right
	left = wordRange->left;
	right = wordRange->right;
	while (left < right) {
		int center = (left + right) >> 1;
		auto thisChar = z::core::toUpper(wordList[center]->at(wordRange->charPos));

		if (thisChar > nextChar)
			right = center - 1;
		else
			left = center + 1;
	}
	if (z::core::toUpper(wordList[right]->at(wordRange->charPos)) < nextChar)
		--right;
	wordRange->right = right;

	if (z::core::toUpper(wordList[wordRange->left]->at(wordRange->charPos)) != nextChar) {
		wordRange->exhausted = true;
	} else {
		++(wordRange->charPos);
		wordRange->isWord = (wordRange->charPos == (wordList[wordRange->left]->length()));
	}

	return !(wordRange->exhausted);
}

dictIter dictionary::begin() const noexcept {
	return dictIter(&wordList, 0);
}

dictIter dictionary::end() const noexcept {
	return dictIter(&wordList, wordList.length());
}

const zstring &dictionary::at(int index) const {
	return *(wordList.at(index));
}

int dictionary::maxWordLength() const noexcept {
	return maxWordLen;
}
} // namespace util
} // namespace z
