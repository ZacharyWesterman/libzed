#include "dictionary.hpp"
#include "../core/charFunctions.hpp"
#include "../core/join.hpp"
#include "../file/exceptions.hpp"
#include "../file/write.hpp"

#include <fstream>

namespace z {
namespace util {

dictionary::dictionary(bool caseSensitive) noexcept : caseSensitive(caseSensitive), readingStream(false), maxWordLen(0) {}

dictionary::dictionary(const dictionary &other) noexcept : core::sortedRefArray<zstring *>(), caseSensitive(other.caseSensitive), readingStream(other.readingStream), maxWordLen(other.maxWordLen) {
	increase(other.length());
	caseSensitive = other.caseSensitive;
	readingStream = other.readingStream;
	maxWordLen = other.maxWordLen;

	for (auto i : other.array_data) {
		append(new zstring(*i));
	}
}

dictionary::~dictionary() noexcept {
	clear();
}

void dictionary::clear() noexcept {
	for (int i = 0; i < length(); i++) {
		delete at(i);
	}
	array_data.clear();
}

bool dictionary::read(const zstring &filename, const core::timeout &time, bool assumePresorted) {
	std::ifstream stream(filename.str());

	if (stream.fail()) {
		throw z::file::unreadable(filename);
	}
	if (stream.eof()) {
		return false;
	}

	if (!readingStream) {
		clear();
		maxWordLen = 0;
		readingStream = true;
	}

	while (!(stream.eof() || time.timedOut())) {
		zstring *word = new zstring;
		word->read(stream);
		if (word->length()) {
			if (!caseSensitive) {
				word->toLower();
			}

			if (assumePresorted) {
				append(word);
			} else {
				add(word);
			}

			maxWordLen = std::max(word->length(), maxWordLen);
		} else {
			delete word;
		}
	}

	return !stream.eof();
}

void dictionary::write(const zstring &filename) {
	z::file::write(z::core::joinDeref(*this, '\n'_zs), filename);
}

bool dictionary::isWord(const zstring &word) const noexcept {
	return find(const_cast<zstring *>(&word)) >= 0;
}

void dictionary::addWord(const zstring &word) noexcept {
	auto wordptr = new zstring(caseSensitive ? word : word.lower());
	maxWordLen = std::max(wordptr->length(), maxWordLen);
	if (find(wordptr) < 0) {
		add(wordptr);
	}
}

bool dictionary::isCaseSensitive() const noexcept {
	return caseSensitive;
}

void dictionary::setCaseSensitive(bool caseSensitive) noexcept {
	this->caseSensitive = caseSensitive;
}

dictRange dictionary::range() const noexcept {
	dictRange wordRange;

	wordRange.left = 0;
	wordRange.right = length() - 1;
	wordRange.charPos = 0;
	wordRange.isWord = false;
	wordRange.exhausted = !length();

	return wordRange;
}

bool dictionary::narrow(dictRange *wordRange, uint32_t nextChar) const noexcept {
	if (wordRange->exhausted || !wordRange) {
		return false;
	}
	wordRange->isWord = false;

	nextChar = z::core::toUpper(nextChar);

	// get furthest left
	int left = wordRange->left;
	int right = wordRange->right;
	while (left < right) {
		int center = (left + right) >> 1;
		auto thisChar = z::core::toUpper(at(center)->at(wordRange->charPos));

		if (thisChar < nextChar) {
			left = center + 1;
		} else {
			right = center - 1;
		}
	}
	if (z::core::toUpper(at(left)->at(wordRange->charPos)) < nextChar) {
		++left;
	}

	// If we've skipped past the last successful match, we're done.
	if (wordRange->charPos) {
		auto oldLast = z::core::toUpper(at(wordRange->left)->at(wordRange->charPos - 1));
		auto newLast = z::core::toUpper(at(left)->at(wordRange->charPos - 1));

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
		auto thisChar = z::core::toUpper(at(center)->at(wordRange->charPos));

		if (thisChar > nextChar) {
			right = center - 1;
		} else {
			left = center + 1;
		}
	}
	if (z::core::toUpper(at(right)->at(wordRange->charPos)) < nextChar) {
		--right;
	}
	wordRange->right = right;

	if (z::core::toUpper(at(wordRange->left)->at(wordRange->charPos)) != nextChar) {
		wordRange->exhausted = true;
	} else {
		++(wordRange->charPos);
		wordRange->isWord = (wordRange->charPos == (at(wordRange->left)->length()));
	}

	return !(wordRange->exhausted);
}

int dictionary::maxWordLength() const noexcept {
	return maxWordLen;
}

dictionary dictionary::filter(std::function<bool(const zstring &)> lambda) const {
	dictionary result;
	result.caseSensitive = caseSensitive;

	result.increase(array_data.size()); // Increase it to the max size for performance, but it will likely be smaller than this.

	for (const zstring *i : array_data) {
		if (lambda(*i)) {
			result.append(new zstring(*i));
		}
	}

	return result;
}

dictionary &dictionary::operator=(const dictionary &other) noexcept {
	clear();
	increase(other.length());

	caseSensitive = other.caseSensitive;
	readingStream = other.readingStream;
	maxWordLen = other.maxWordLen;

	for (auto i : other.array_data) {
		append(new zstring(*i));
	}

	return *this;
}

dictionary &dictionary::operator=(dictionary &&other) noexcept {
	clear();
	array_data = other.array_data;
	caseSensitive = other.caseSensitive;
	readingStream = other.readingStream;
	maxWordLen = other.maxWordLen;

	other.array_data.clear();

	return *this;
}

} // namespace util
} // namespace z
