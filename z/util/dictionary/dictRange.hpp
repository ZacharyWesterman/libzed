#pragma once

namespace z {
namespace util {
class dictionary;

/**
 * @brief A class to allow efficient, custom dictionary search functions.
 *
 * @note This class has no use on its own, it is meant to be used in
 * conjunction with z::util::dictionary.
 */
class dictRange {
protected:
	/// The leftmost word in the current range.
	int left;

	/// The rightmost word in the current range.
	int right;

	/// The index of the next character to compare.
	int charPos;

public:
	/// Whether the range can be narrowed any further.
	bool exhausted;

	/// Whether the leftmost side of the current range happens to be a word in the dictionary.
	bool isWord;

	/// z::util::dictionary is allowed to modify
	friend dictionary;
};
} // namespace util
} // namespace z
