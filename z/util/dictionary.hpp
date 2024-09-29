#pragma once

#include "../core/sortedRefArray.hpp"
#include "../core/string.hpp"
#include "../core/timeout.hpp"

#include "../core/sortedRefArray.hpp"
#include "dictionary/dictRange.hpp"

#include <istream>

namespace z {
namespace util {
/**
 * @brief A class for performing searches on a dictionary of words.
 */
class dictionary : public core::sortedRefArray<zstring *> {
private:
	bool caseSensitive;
	bool readingStream;
	int maxWordLen;

public:
	/**
	 * @brief Constructor
	 * @param caseSensitive Whether searches on this dictionary are case sensitive.
	 */
	dictionary(bool caseSensitive = false) noexcept;

	/**
	 * @brief Copy constructor
	 * @param other The dictionary to copy from.
	 */
	dictionary(const dictionary &other) noexcept;

	/// Destructor
	~dictionary() noexcept;

	/**
	 * @brief Empty the word list.
	 */
	void clear() noexcept;

	/**
	 * @brief Read this dictionary's word list as text from a stream.
	 *
	 * Note that for long dictionaries, it may take
	 * several seconds for this operation to complete!
	 * Hence the timeout method.
	 *
	 * @param stream The stream to read from.
	 * @param time Optional param to force return after time out.
	 * @param assumePresorted If false, the word list is sorted while it is read.
	 * If true, no sorting is done.
	 *
	 * @note Operating on an un-sorted dictionary may have undefined behavior!
	 * Only set assumePresorted to true if you know for sure your data is sorted already!
	 *
	 * @return A positive integer if finished reading successfully, 0 if not finished reading,
	 * a negative integer if read failed.
	 *
	 * @threadsafe_member_no
	 */
	int read(std::istream &stream, const core::timeout &time = -1, bool assumePresorted = false) noexcept;

	/**
	 * @brief Check if the given string is a valid word in the dictionary (case is ignored).
	 *
	 * @param word The word to search for.
	 *
	 * @return True if it is a valid word, false otherwise.
	 *
	 * @threadsafe_member_yes
	 */
	bool isWord(const zstring &word) const noexcept;

	/**
	 * @brief Add a word to the dictionary.
	 *
	 * If the word already exists, does nothing. Otherwise, creates a new word.
	 *
	 * @param word The word to add or update.
	 *
	 * @threadsafe_member_no
	 */
	void addWord(const zstring &word) noexcept;

	/**
	 * @brief Check whether this dictionary is case sensitive.
	 *
	 * @return true if case sensitive, false otherwise.
	 *
	 * @threadsafe_member_yes
	 */
	bool isCaseSensitive() const noexcept;

	/**
	 * @brief Set the case sensitivity of this dictionary.
	 *
	 * @param caseSensitive Whether this dictionary is case sensitive.
	 */
	void setCaseSensitive(bool caseSensitive) noexcept;

	/**
	 * @brief Create a new range of words encompassing the whole dictionary.
	 *
	 * This is meant to generate the initial object, then narrow() should be
	 * called to narrow down the possible results.
	 *
	 * @return An empty dictRange object.
	 */
	dictRange range() const noexcept;

	/**
	 * @brief Narrow the results of a range given the next character.
	 *
	 * @param wordRange A reference to the range we're narrowing.
	 * @param nextChar The next character all words in the range must contain.
	 *
	 * @return true if the range can be narrowed further, false otherwise.
	 */
	bool narrow(dictRange *wordRange, uint32_t nextChar) const noexcept;

	/**
	 * @brief Get the length of the longest word in the dictionary.
	 *
	 * This function is very fast, as the value is calculated when the dictionary is
	 * loaded or changed, \b not when this function is called. Prefer this function over
	 * manually scanning the dictionary for the longest word.
	 *
	 * @return The length of the longest word.
	 */
	int maxWordLength() const noexcept;

	/**
	 * @brief Filters the dictionary based on a predicate and returns a new dictionary containing the words that satisfy the predicate.
	 *
	 * This function iterates through the dictionary, applies the given lambda function as a predicate to each element,
	 * and adds words that satisfy the predicate to the resulting dictionary.
	 *
	 * @param lambda A function that takes a constant reference to a zstring and returns a boolean indicating whether
	 * the element should be included.
	 * @return A new dictionary containing the words that satisfy the predicate.
	 */
	dictionary filter(std::function<bool(const zstring &)> lambda) const;

	/**
	 * @brief Dictionary move operator.
	 *
	 * Move a dictionary's contents to this dictionary.
	 *
	 * @param other the dictionary to copy from.
	 *
	 * @return This dictionary after the operation (for
	 * @b a=b=c type expressions).
	 */
	dictionary &operator=(dictionary &&other) noexcept;

	/**
	 * @brief Dictionary assignment operator.
	 *
	 * Clear the contents of this dictionary and create
	 * a copy of another dictionary's contents into this one.
	 *
	 * @param other the dictionary to copy from.
	 *
	 * @return This dictionary after the operation (for
	 * @b a=b=c type expressions).
	 */
	dictionary &operator=(const dictionary &other) noexcept;

#ifdef __has_include
#if __has_include(<cereal/cereal.hpp>)
	/**
	 * @brief Serialization output.
	 * @param ar The output archive.
	 */
	template <typename archive>
	void save(archive &ar) const {
		ar((CEREAL_SIZE_TYPE)length());
		for (int i = 0; i < length(); i++) {
			ar(*at(i));
		}
	}

	/**
	 * @brief Serialization input.
	 * @param ar The input archive.
	 */
	template <class archive>
	void load(archive &ar) {
		clear();
		CEREAL_SIZE_TYPE sz;
		ar(sz);
		increase(sz);

		for (CEREAL_SIZE_TYPE i = 0; i < sz; i++) {
			zstring *data = new zstring;
			ar(*data);
			append(data);
		}
	}
#endif
#endif
};
} // namespace util
} // namespace z
