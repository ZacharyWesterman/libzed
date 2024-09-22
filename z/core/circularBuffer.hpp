#pragma once

#include "arrayLike.hpp"
#include <type_traits>

namespace z {
namespace core {
/**
 * @brief A custom iterator for circular buffers.
 *
 * This iterator allows C++'s range based for loop syntax.
 */
template <typename TYPE, unsigned int LEN>
class circularIterator {
	const TYPE *ptr;
	int offset;

public:
	/**
	 * @brief Constructor
	 * @param buffer The raw buffer data.
	 * @param index The current index in the buffer.
	 */
	circularIterator(const TYPE *buffer, int index) noexcept : ptr(buffer), offset(index) {}

	/**
	 * @brief Move to the next spot in the buffer.
	 * @return A new iterator at the next index.
	 */
	circularIterator operator++() noexcept {
		++offset;
		return *this;
	}

	/**
	 * @brief Inequality operator.
	 * @param other The other iterator to compare against.
	 * @return True if these iterators are not pointing at the same item, false otherwise.
	 */
	bool operator!=(const circularIterator &other) const noexcept {
		return offset != other.offset;
	}

	/**
	 * @brief Dereference operator.
	 * @return The data at the current index.
	 */
	const TYPE &operator*() const noexcept {
		return ptr[offset % LEN];
	}
};

/**
 * @brief A circular buffer of fixed size.
 * Appending can be done indefinitely,
 * as the index will just loop back around to the beginning.
 */
template <typename TYPE, unsigned int LEN>
class circularBuffer : public sizable, public arrayLike<const TYPE &, circularIterator<TYPE, LEN>> {
	TYPE data[LEN];	 // The data to store
	int counter = 0; // The current index. This will always be between 0 and LEN.
	int total = 0;	 // The total number of items in the buffer. This will never exceed LEN.

	static_assert(LEN > 0, "Buffer must have more than zero elements.");

public:
	/// Default constructor.
	circularBuffer() noexcept {}

	/**
	 * @brief Initialize the buffer with the contents of an array.
	 *
	 * This allows for brace-enclosed initialization e.g.
	 * `circularBuffer<int, 3> buffer = {1, 2, 3};`
	 *
	 * @param args A list of elements to append to the buffer.
	 */
	template <typename... ARGS>
	circularBuffer(const ARGS &...args) noexcept {
		populate(args...);
	}

	/**
	 * @brief Initialize all elements with a single value.
	 * @param default_value The value to initialize elements with.
	 */
	circularBuffer(TYPE default_value) noexcept {
		for (int i = 0; i < LEN; ++i) {
			data[i] = default_value;
		}
	}

	size_t size() const noexcept override {
		return sizeof(TYPE) * LEN;
	}

	int length() const noexcept override {
		return LEN;
	}

	/**
	 * @brief Get an element from the buffer.
	 *
	 * This method is used when we're not modifying the resultant value.
	 * @param index The index of the value we want.
	 * @return The value at that index.
	 */
	const TYPE &at(const int index) const noexcept override {
		return data[index % LEN];
	}

	// Get a reference to an element from the buffer. This is used when we ARE modifying it.
	// The compiler knows the difference and will use this function only when needed.

	/**
	 * @brief Get a modifiable element from the buffer.
	 *
	 * This method is used when we ARE modifying the resultant value.
	 * The compiler knows the difference and will use this function only when needed.
	 *
	 * @param index The index of the value we want.
	 * @return The value at that index.
	 */
	TYPE &at(const int index) noexcept {
		return data[index % LEN];
	}

	/**
	 * @brief Add a value to the buffer, and increment the current index.
	 * @param value The value to append.
	 */
	void append(const TYPE value) noexcept {
		data[counter] = value;
		counter = (counter + 1) % LEN;
		if (total < LEN) {
			++total;
		}
	}

	/**
	 * @brief Get the index of the current top of the buffer.
	 *
	 * This index will always be a value from 0 to the length of the buffer.
	 * @return The current index.
	 */
	int index() const noexcept {
		return counter;
	}

	/**
	 * @brief Get a reference to the topmost item in the buffer.
	 * @return The topmost item.
	 */
	TYPE &current() noexcept {
		return data[counter];
	}

	/**
	 * @brief Get the total number of items in the buffer.
	 *
	 * This will return at most the maximum buffer size.
	 *
	 * @return The number of items in the buffer.
	 */
	int count() const noexcept {
		return total;
	}

	/// Move current "top" to the previous spot in the buffer.
	void prev() noexcept {
		counter = (counter + LEN - 1) % LEN;
	}

	/// Move current "top" to the next spot in the buffer.
	void next() noexcept {
		counter = (counter + 1) % LEN;
	}

	circularIterator<TYPE, LEN> begin() const noexcept override {
		return circularIterator<TYPE, LEN>(data, counter + LEN + LEN - total);
	}

	circularIterator<TYPE, LEN> end() const noexcept override {
		return circularIterator<TYPE, LEN>(data, counter + LEN + LEN);
	}

	/**
	 * @brief Append an arbitrary number of elements to the buffer.
	 * @param first The first element to append.
	 * @param args All other elements to append.
	 */
	template <typename... ARGS>
	void populate(const TYPE &first, const ARGS &...args) noexcept {
		append(first);
		populate(args...);
	}

	/**
	 * @brief Append an element to the buffer.
	 *
	 * This is the tail end of calling populate() with 2 or more arguments.
	 *
	 * @param arg The element to append.
	 */
	void populate(const TYPE &arg) noexcept {
		append(arg);
	}

	/**
	 * @brief Get the minimum value contained in this buffer.
	 * @note This method is only available for arithmetic types.
	 * @return The minimum value in the buffer.
	 */
	TYPE minimum() const noexcept {
		static_assert(std::is_arithmetic<TYPE>::value, "circularBuffer::minimum() is only defined for arithmetic types.");

		TYPE value = data[0];
		for (int i = 1; i < LEN; ++i) {
			if (data[i] < value) {
				value = data[i];
			}
		}
		return value;
	}

	/**
	 * @brief Get the maximum value contained in this buffer.
	 * @note This method is only available for arithmetic types.
	 * @return The maximum value in the buffer.
	 */
	TYPE maximum() const noexcept {
		static_assert(std::is_arithmetic<TYPE>::value, "circularBuffer::maximum() is only defined for arithmetic types.");

		TYPE value = data[0];
		for (int i = 1; i < LEN; ++i) {
			if (data[i] > value) {
				value = data[i];
			}
		}
		return value;
	}
};
} // namespace core
} // namespace z
