#pragma once

#include "../encoding.hpp"
#include <cstddef>
#include <cstdint>

namespace z {
namespace core {
/**
 * \brief A class for iterating over each character in a string.
 *
 * Since some string encoding schemes (i.e. UTF-8) have multi-character sequences
 * That represent a single "character", this iterator allows for looping over each
 * character in strings of different encoding identically.
 */
template <encoding E>
class stringIterator {
private:
	uint8_t *data;
	uint32_t chr;

public:
	/**
	 * \brief Constructor
	 *
	 * \param ptr Pointer to the string data.
	 * \param offset The current index in the string data.
	 */
	stringIterator(uint8_t *ptr, size_t offset) noexcept;

	/**
	 * \brief Increment the current character we're pointing to.
	 *
	 * \return A new iterator instance, incremented from the current position.
	 */
	stringIterator operator++() noexcept;

	/**
	 * \brief Equality operator
	 *
	 * \param other The object to compare to.
	 *
	 * \return True if this and other are pointing to the same character in the same string, false otherwise.
	 */
	bool operator!=(const stringIterator &other) const noexcept {
		return data != other.data;
	}

	/**
	 * \brief Dereference operator.
	 *
	 * \return The current character this iterator is pointing to, converted to UTF 32.
	 */
	const uint32_t &operator*() const noexcept {
		return chr;
	}
};
} // namespace core
} // namespace z
