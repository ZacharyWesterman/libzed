#pragma once

#include <complex>

#include "sizable.hpp"
#include "stringIterator.hpp"
#include "zstr.hpp"

namespace z
{
	namespace core
	{
		class stringBase : public sizable
		{
		protected:
			uint8_t* data;
			int data_len;
			int character_ct;

			virtual uint32_t getChar(int index) const noexcept = 0;
			virtual void setChar(uint32_t value, int index) noexcept = 0;
			virtual int charPower() const noexcept = 0;

		public:
			/**
			* \brief Increase the space allocated for this string.
			*
			* If this string currently has fewer than max_chars allocated,
			* enough space is reallocated to hold at least that many characters.
			* Note that strings follow an approximate 1.5x growth pattern.
			*
			* \param charCount The minimum number of characters this string should be able to contain before reallocating.
			*/
			virtual void increase(int charCount) noexcept = 0;

			///Destructor
			~stringBase();

			/**
			* \brief Get the character at the given index.
			*
			* \param index The index of the character to get.
			*
			* \return The character at the given index, in UTF32 format.
			*
			* If the index is greater than the character count, the
			* null character is returned.
			* For UTF-8 strings, if the chracter at the given index is
			* part of a multibyte sequence, that sequence is converted to
			* UTF32 and returned.
			*
			* \see operator[]()
			*/
			uint32_t at(int index) const noexcept;

			/**
			* \brief Get the character at the given index.
			*
			* \param index The index of the character to get.
			*
			* \return The character at the given index, in UTF32 format.
			*
			* If the index is greater than the character count, the
			* null character is returned.
			* For UTF-8 strings, if the chracter at the given index is
			* part of a multibyte sequence, that sequence is converted to
			* UTF32 and returned.
			*
			* \see at()
			*/
			uint32_t operator[](int index) const noexcept;

			/**
			* \brief Get the size of the string in memory
			*
			* \return The number of bytes in memory this string currently consumes.
			*/
			size_t size() const noexcept;

			/**
			* \brief Get the character count of the string.
			*
			* \return The number of characters in the string.
			*
			* For UTF-8 strings, multibyte sequences are counted
			* as multiple characters.
			*
			* \see chars()
			*/
			int length() const noexcept;

			/**
			* \brief Convert this string to an integer.
			*
			* \param base The base this string is being interpreted as.
			* \param decimal The character used to denote the radix (decimal place).
			*
			* \return A signed integer representation of this string, if it
			* is a valid integer. 0 otherwise.
			*
			* \note If a radix character is encountered before any invalid characters,
			* any text following the radix is ignored.
			*
			* \see isInteger()
			*/
			long integer(int base = 10, uint32_t decimal = '.') const noexcept;

			/**
			* \brief Convert this string to a floating-point value.
			*
			* \param base The base this string is being interpreted as.
			* \param decimal The character used to denote the radix (decimal place).
			*
			* \return The floating-point representation of this string, if it
			* is a valid float. 0 otherwise.
			*
			* \see isFloating()
			*/
			double floating(int base = 10, uint32_t decimal = '.') const noexcept;

			/**
			* \brief Convert this string to a complex value.
			*
			* \param base The base this string is being interpreted as.
			* \param decimal The character used to denote the radix (decimal place).
			*
			* \return The floating-point complex representation of this string,
			* if it is a valid complex float. 0 otherwise.
			*
			* \see isComplex()
			*/
			std::complex<double> complex(int base = 10, uint32_t decimal = '.') const noexcept;

			/**
			* \brief Convert this string to an integer.
			* \see integer()
			*/
			template <typename INT, typename = typename std::enable_if<std::is_integral<INT>::value,INT>::type>
			explicit operator INT() const noexcept {return integer();}

			/**
			* \brief Convert this string to a floating-point value.
			* \see floating()
			*/
			explicit operator double() const noexcept {return floating();}

			/**
			* \brief Convert this string to a floating-point value.
			* \see floating()
			*/
			explicit operator float() const noexcept {return floating();}

			/**
			* \brief Get the encoding of this string.
			*
			* \return An integer indicating this string's encoding.
			*
			* \see encoding.h
			*/
			virtual encoding format() const noexcept = 0;

			/**
			* \brief Get a sub-string from this string.
			*
			* \param index The sub-string starting index.
			* \param count The number of characters in the sub-string.
			*
			* \return A subset of the original string.
			*
			* If count is 0, a null string is returned. If count is negative, characters are
			* added in reverse order. <br/>
			* e.g. for `string str = "example"`, `str.substr(2,3)` will
			* give `"amp"`, and `str.substr(5,-3)` will give `"pma"`.
			*/
			virtual stringBase substr(int index, int count) const noexcept = 0;

			/**
			* \brief Count the occurrences of a sub-string.
			*
			* \param other The sub-string to search for.
			*
			* \return The number of times the given sub-string appears in
			* the full string.
			*/
			int count(const stringBase& other) const noexcept;

			/**
			* \brief Find a specific occurrence of a sub-string.
			*
			* \param other The sub-string to search for.
			* \param occurrence The occurrence of the sub-string to find.
			*
			* \return The index of the given occurrence of the sub-string,
			* if that occurrence was found. Otherwise, -1.
			*
			* Attempts to find given occurrence of the sub-string starting at
			* the \b beginning of the string. Note that the occurrence starts at 1.
			* If the occurrence is less than 1, then -1 is returned.
			*/
			int find(const stringBase& other, int occurrence = 1) const noexcept;

			/**
			* \brief Check if a specific occurrence of a sub-string exists.
			*
			* \param other The sub-string to search for.
			* \param occurrence The occurrence of the sub-string to find.
			*
			* \return True if the given occurrence of the sub-string was found, false otherwise.
			*
			* Attempts to find given occurrence of the sub-string starting at
			* the \b beginning of the string. Note that the occurrence starts at 1.
			* If the occurrence is less than 1, then false is returned.
			*/
			int found(const stringBase& other, int occurrence = 1) const noexcept
			{
				return this->findAfter(other,0,occurrence) >= 0;
			}

			/**
			* \brief Reverse-find a specific occurrence of a sub-string.
			*
			* \param other The sub-string to search for.
			* \param occurrence The occurrence of the sub-string to find.
			*
			* \return The index of the given occurrence of the sub-string,
			* if that occurrence was found. Otherwise, -1.
			*
			* Attempts to find given occurrence of the sub-string starting at
			* the \b end of the string. Note that the occurrence starts at 1.
			* If the occurrence is less than 1, then -1 is returned.
			*/
			int findLast(const stringBase& other, int occurrence = 1) const noexcept
			{
				return this->findBefore(other, this->character_ct, occurrence);
			}

			/**
			* \brief Find a specific occurrence of a sub-string.
			*
			* \param other The sub-string to search for.
			* \param index The index to start at.
			* \param occurrence The occurrence of the sub-string to find.
			*
			* \return The index of the given occurrence of the sub-string,
			* if that occurrence was found. Otherwise, -1.
			*
			* Attempts to find given occurrence of the sub-string, searching
			* forward starting at the given index. Note that the occurrence starts at 1.
			* If the occurrence is less than 1, then -1 is returned.
			*/
			int findAfter(const stringBase& other, int index, int occurrence = 1) const noexcept;

			/**
			* \brief Reverse-find a specific occurrence of a sub-string.
			*
			* \param other The sub-string to search for.
			* \param index The index to start at.
			* \param occurrence The occurrence of the sub-string to find.
			*
			* \return The index of the given occurrence of the sub-string,
			* if that occurrence was found. Otherwise, -1.
			*
			* Attempts to find given occurrence of the sub-string, searching
			* backward starting at the given index. Note that the occurrence starts at 1.
			* If the occurrence is less than 1, then -1 is returned.
			*/
			int findBefore(const stringBase& other, int index, int occurrence = 1) const noexcept;

			/**
			* \brief Check if a sub-string is found at the given index.
			*
			* \param other The sub-string to check for.
			* \param index The index in the string to look at.
			*
			* \return True if the sub-string was found beginning at the given index.
			* False otherwise.
			*/
			bool foundAt(const stringBase& other, int index) const noexcept;

			/**
			* \brief Check if a sub-string ends at the given index.
			*
			* \param other The sub-string to check for.
			* \param index The index in the string to look at.
			*
			* \return True if the sub-string was found ending at the given index.
			* False otherwise.
			*/
			bool foundEndAt(const stringBase& other, int index) const noexcept;

			/**
			* \brief Check if the string begins with a given sub-string.
			*
			* \param other The sub-string to check for.
			*
			* \return True if this string begins with the given sub-string.
			* False otherwise.
			*/
			bool beginsWith(const stringBase& other) const noexcept
			{
				return this->foundAt(other, 0);
			}

			/**
			* \brief Check if the string ends with a given sub-string.
			*
			* \param other The sub-string to check for.
			*
			* \return True if this string ends with the given sub-string.
			* False otherwise.
			*/
			bool endsWith(const stringBase& other) const noexcept
			{
				return this->foundEndAt(other, character_ct-1);
			}

			/**
			* \brief Check if this string can convert to an integer.
			*
			* \param base The base this string is being interpreted as.
			*
			* \return True if the string contains only characters representing a
			* signed integer. False otherwise.
			*
			* Valid strings may contain only numeric characters in the given base,
			* and may optionally begin with a `+` or `-`.
			*
			* \see integer()
			*/
			bool isInteger(int base = 10) const noexcept
			{
				return type(base) & (zstr::integer);
			}

			/**
			* \brief Check if this string can convert to a floating-point value.
			*
			* \param base The base this string is being interpreted as.
			* \param decimal The character that is used as a decimal separator.
			*
			* \return True if the string contains only characters representing a
			* float. False otherwise.
			*
			* Valid strings may optionally begin with a `+` or `-`, and contain only one
			* period (decimal point). The number may also have an exponent in the same base,
			* in the form `XXeY`, where the exponent `Y` may optionally begin with a `+` or `-`.
			* Unlike the main number, this exponent must be an integer.
			*
			* \see floating()
			*/
			bool isFloating(int base = 10, uint32_t decimal = '.') const noexcept
			{
				return type(base,decimal) & (zstr::integer | zstr::floating);
			}

			/**
			* \brief Check if this string can convert to a complex value.
			*
			* \param base The base this string is being interpreted as.
			* \param decimal The character that is used as a decimal separator.
			*
			* \return True if the string contains only characters representing a
			* floating-point complex number. False otherwise.
			*
			* Valid strings must follow the form `X+Yi` or `Xi+Y`, where `X` and `Y` are
			* floating-point values. `X` may optionally have a `+` or `-` preceding it,
			* and `Y` may be preceded by a `-` instead of a `+`.
			*
			* \see isFloating()
			* \see complex()
			*/
			bool isComplex(int base = 10, uint32_t decimal = '.') const noexcept
			{
				return type(base,decimal) & (zstr::integer | zstr::floating | zstr::complex);
			}

			/**
			* \brief Determine the most basic type that this string can cast to.
			*
			* \param base The base that numbers are assumed to be in.
			* \param decimal The character that is used as a decimal separator.
			*
			* \return A value from the zstr enum representing the type.
			*
			* \see isInteger()
			* \see isFloating()
			* \see isComplex()
			* \see isString()
			*/
			int type(int base = 10, uint32_t decimal = '.') const noexcept;

			/**
			* \brief Reset to a null string.
			*/
			void clear() noexcept;

			/**
			* \brief Check if this string contains any characters in the given range.
			*
			* example: this->contains('A', 'Z') will return true if this string contains a character in the
			* range A -> Z, inclusive.
			*
			* \param first The first character in the range.
			* \param last The last character in the range.
			* \param exclusive Return false if this string contains any characters NOT in the given string.
			*
			* \return Whether this string contained a character in the given range, or if the `exclusive` flag is set,
			* whether it contains ONLY characters in the given range.
			*/
			bool contains(uint32_t first, uint32_t last, bool exclusive = false) const noexcept
			{
				return contains({first, last}, exclusive);
			}

			/**
			* \brief Check if this string contains any characters in the given range.
			*
			* example: this->contains({'A', 'Z'}) will return true if this string contains a character in the
			* range A -> Z, inclusive.
			*
			* \param range A std::pair denoting the character range.
			* \param exclusive Return false if this string contains any characters NOT in the given string.
			*
			* \return Whether this string contained a character in the given range, or if the `exclusive` flag is set,
			* whether it contains ONLY characters in the given range.
			*/
			bool contains(const std::pair<uint32_t, uint32_t>& range, bool exclusive = false) const noexcept
			{
				return contains({range}, exclusive);
			}

			/**
			* \brief Check if this string contains any characters in the given ranges.
			*
			* example: this->contains({{'A', 'Z'}, {'a', 'z'}, {'0', '9'}}) will return true if this string
			* contains an alphanumeric character.
			*
			* \param list A list of std::pair objects denoting all valid character ranges.
			* \param exclusive Return false if this string contains any characters NOT in any of the given ranges.
			*
			* \return Whether this string contained a character in any of the given ranges, or if the `exclusive` flag is set,
			* whether it contains ONLY characters in the given ranges.
			*/
			bool contains(const std::initializer_list<const std::pair<uint32_t, uint32_t>>& list, bool exclusive = false) const noexcept
			{
				for (uint32_t chr : *this)
				{
					bool matched = false;
					for (auto& range : list)
					{
						if ((chr >= range.first) && (chr <= range.second))
						{
							matched = true;
							break;
						}
					}

					//if (matched && !exclusive) or (!matched && exclusive)
					//then we know the result, exit right away.
					if (matched ^ exclusive)
					{
						return matched;
					}
				}

				//will ony get here if (!matched && !exclusive) or (matched && exclusive)
				//therefore, matched := exclusive
				return exclusive;
			}

			/**
			* \brief Check if this string contains any of the characters in the given string.
			*
			* example: this->contains("aeiouyAEIOUY") will return true if this contains a vowel.
			*
			* \param list A string indicating the list of valid characters.
			* \param exclusive Return false if this string contains any characters NOT in the given string.
			*
			* \return Whether this string contained any of the characters in the given string, or if the `exclusive` flag is set,
			* whether it contains ONLY characters in the given string.
			*/
			bool contains(const stringBase& list, bool exclusive = false) const noexcept;

			stringIterator begin() const noexcept;

			stringIterator end() const noexcept;
		};
	}
}
