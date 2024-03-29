#pragma once

#include <type_traits>
#include "arrayLike.hpp"

namespace z
{
	namespace core
	{
		/**
		 * \brief A circular buffer of fixed size.
		 * Appending can be done indefinitely,
		 * as the index will just loop back around to the beginning.
		 */
		template<typename TYPE, unsigned int LEN>
		class circularBuffer : public sizable, public arrayLike<const TYPE&>
		{
			TYPE data[LEN];  //The data to store
			int counter = 0; //The current index. This will always be between 0 and LEN.
			int total = 0;   //The total number of items in the buffer. This will never exceed LEN.

			static_assert(LEN > 0, "Buffer must have more than zero elements.");

			//A custom iterator for this class, to allow C++'s range based for loop syntax
			class iterator {
				const TYPE* ptr;
				int offset;

			public:
				iterator(const TYPE* buffer, int index) noexcept : ptr(buffer), offset(index) {}

				iterator operator++() noexcept
				{
					++offset;
					return *this;
				}

				bool operator!=(const iterator& other) const noexcept
				{
					return offset != other.offset;
				}

				const TYPE& operator*() const noexcept
				{
					return ptr[offset % LEN];
				}
			};

		public:
			///Default constructor.
			circularBuffer() noexcept {}

			/**
			 * \brief Initialize the buffer with the contents of an array.
			 *
			 * This allows for brace-enclosed initialization e.g.
			 * `circularBuffer<int, 3> buffer = {1, 2, 3};`
			 *
			 * \param args A list of elements to append to the buffer.
			 */
			template<typename... ARGS>
			circularBuffer(const ARGS&... args) noexcept
			{
				populate(args...);
			}

			/**
			 * \brief Initialize all elements with a single value.
			 * \param default_value The value to initialize elements with.
			 */
			circularBuffer(TYPE default_value) noexcept
			{
				for (int i = 0; i < LEN; ++i)
				{
					data[i] = default_value;
				}
			}

			constexpr size_t size() const noexcept override
			{
				return sizeof(TYPE) * LEN;
			}

			constexpr int length() const noexcept override
			{
				return LEN;
			}

			/**
			 * \brief Get an element from the buffer.
			 *
			 * This method is used when we're not modifying the resultant value.
			 * \param index The index of the value we want.
			 * \return The value at that index.
			 */
			const TYPE& at(const int index) const noexcept override
			{
				return data[index % LEN];
			}

			//Get a reference to an element from the buffer. This is used when we ARE modifying it.
			//The compiler knows the difference and will use this function only when needed.

			/**
			 * \brief Get a modifiable element from the buffer.
			 *
			 * This method is used when we ARE modifying the resultant value.
			 * The compiler knows the difference and will use this function only when needed.
			 *
			 * \param index The index of the value we want.
			 * \return The value at that index.
			 */
			TYPE& at(const int index) noexcept
			{
				return data[index % LEN];
			}

			/**
			 * \brief Add a value to the buffer, and increment the current index.
			 * \param value The value to append.
			 */
			void append(const TYPE value) noexcept
			{
				data[counter] = value;
				counter = (counter + 1) % LEN;
				if (total < LEN) ++total;
			}

			/**
			 * \brief Get the index of the current top of the buffer.
			 *
			 * This index will always be a value from 0 to the length of the buffer.
			 * \return The current index.
			 */
			int index() const noexcept
			{
				return counter;
			}

			/**
			 * \brief Get a reference to the topmost item in the buffer.
			 * \return The topmost item.
			 */
			TYPE& current() noexcept
			{
				return data[counter];
			}

			/**
			 * \brief Get the total number of items in the buffer.
			 *
			 * This will return at most the maximum buffer size.
			 *
			 * \return The number of items in the buffer.
			 */
			int count() const noexcept
			{
				return total;
			}

			///Move current "top" to the previous spot in the buffer.
			void prev() noexcept
			{
				counter = (counter + LEN - 1) % LEN;
			}

			///Move current "top" to the next spot in the buffer.
			void next() noexcept
			{
				counter = (counter + 1) % LEN;
			}

			iterator begin() const noexcept override
			{
				return iterator(data, counter + LEN + LEN - total);
			}

			iterator end() const noexcept override
			{
				return iterator(data, counter + LEN + LEN);
			}

			/**
			 * \brief Append an arbitrary number of elements to the buffer.
			 * \param first The first element to append.
			 * \param args All other elements to append.
			 */
			template<typename... ARGS>
			void populate(const TYPE& first, const ARGS&... args) noexcept
			{
				append(first);
				populate(args...);
			}

			/**
			 * \brief Append an element to the buffer.
			 *
			 * This is the tail end of calling populate() with 2 or more arguments.
			 *
			 * \param arg The element to append.
			 */
			void populate(const TYPE& arg) noexcept
			{
				append(arg);
			}

			/**
			 * \brief Get the minimum value contained in this buffer.
			 * \note This method is only available for arithmetic types.
			 * \return The minimum value in the buffer.
			 */
			TYPE minimum() const noexcept
			{
				static_assert(std::is_arithmetic<TYPE>::value, "circularBuffer::minimum() is only defined for arithmetic types.");

				TYPE value = data[0];
				for (int i = 1; i < LEN; ++i)
				{
					if (data[i] < value) value = data[i];
				}
				return value;
			}

			/**
			 * \brief Get the maximum value contained in this buffer.
			 * \note This method is only available for arithmetic types.
			 * \return The maximum value in the buffer.
			 */
			TYPE maximum() const noexcept
			{
				static_assert(std::is_arithmetic<TYPE>::value, "circularBuffer::maximum() is only defined for arithmetic types.");

				TYPE value = data[0];
				for (int i = 1; i < LEN; ++i)
				{
					if (data[i] > value) value = data[i];
				}
				return value;
			}
		};
	}
}
