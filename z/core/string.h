#pragma once

#include <type_traits>
#include <complex>
#include <climits>

#include "charFunctions.h"
#include <z/encoding.h>
#include "serializable.h"

#ifndef Z_STR_INT_BUFSIZE
	#define Z_STR_INT_BUFSIZE 64
#endif

#ifndef Z_STR_FLOAT_BUFSIZE
	#define Z_STR_FLOAT_BUFSIZE 64
#endif

#ifndef Z_STR_EXP_BUFSIZE
	#define Z_STR_EXP_BUFSIZE 10
#endif

#ifndef Z_STR_FLOAT_PRECISION
	#define Z_STR_FLOAT_PRECISION 6
#endif

#ifndef Z_STR_POINTER_FORCE
	#define Z_STR_POINTER_FORCE true
#endif

#ifndef Z_STR_POINTER_CHARS
	#define Z_STR_POINTER_CHARS 8
#endif


static size_t integralBuf(unsigned long integral, int base, uint8_t* buf);
static size_t fractionalBuf(double fractional, int base, int precision, bool force, uint8_t* buf);

namespace z
{
	namespace core
	{
		/**
         * \brief A template class for character strings.
         *
         * This class focuses on how characters are encoded rather than character size.
		 * Possible encoding schemes are ASCII, UTF-8, UTF16, and UTF32.
		 * When characters are input or output, they are converted to or from their
		 * encoding scheme to a single UTF32 character. Thus, the "default" character
		 * type is `uint32_t`.
         * <br/><br/>
		 * Allocated memory is increased as needed with approximate 1.5x growth, and
		 * is not decreased on subsequent data changes, except in the case where
		 * data is copied over to a different string.
		 * <br/><br/>
		 * <B>RE-ENTRANCE:</B><br/>
		 * Simultaneous accesses to the same object can cause data races.
         */
		template <encoding E>
		class string : public serializable
		{
			friend string<ascii>;
			friend string<utf8>;
			friend string<utf16>;
			friend string<utf32>;

		private:
			uint8_t* data;
			size_t data_len;
			size_t character_ct;

			void initChar(uint32_t, size_t);
			void increase(size_t); //increase number of data bytes up to the given amount
			constexpr size_t charSize() const;

		public:
			///Default string constructor
			string();

			/**
			 * \brief Construct string from a single-byte character.
			 *
			 * \param chr Initializing character.
			 *
			 * Character is assumed to be compatible with this string's encoding.
			 */
			string(char chr);

			/**
			 * \brief Construct string from a wide character.
			 *
			 * \param chr Initializing character.
			 *
			 * Converts the given character to the appropriate encoding for this string.
			 */
			string(wchar_t chr);

			/**
			 * \brief Construct string from uint32_t.
			 *
			 * \param chr Initializing character.
			 *
			 * This constructor exists to allow a "default" string character to
			 * again be constructed into a string.
			 * Converts the given character to the appropriate encoding for this string.
			 *
			 * \see at()
			 * \see operator[]()
			 */
			string(const uint32_t& chr);

			/**
			 * \brief Construct from a cstring of single-byte characters.
			 *
			 * \param str Null-terminated cstring.
			 *
			 * All characters are assumed to be compatible with this string's encoding.
			 */
			string(const char* str);

			/**
			 * \brief Construct from a cstring of wide characters.
			 *
			 * \param str Null-terminated cstring.
			 *
			 * Converts the characters in the given cstring to the appropriate encoding
			 * for this string.
			 */
			string(const wchar_t* str);

			/**
			 * \brief Construct from an integer.
			 *
			 * \param value An integer.
			 * \param base The number's base.
			 * \param padSize Number of characters to pad up to.
			 *
			 * Converts an integer to a string in the given base.
			 * If the character count is less than the pad size, zeroes
			 * are added to the left side until the character count equals
			 * the pad size. Valid base sizes are from 2 to 36, and anything
			 * else is assumed to be base 10. If the pad size is less than 1,
			 * no character padding is applied.
			 */
			template <typename INT, typename = typename std::enable_if<std::is_integral<INT>::value,INT>::type>
			string(INT value, int base = 10, int padSize = 0);

			/**
			 * \brief Construct from a pointer.
			 *
			 * \param pointer A pointer.
			 *
			 * Creates a string representation from a pointer, of the form
			 * `0xFFFFFFFF`. If `Z_STR_POINTER_FORCE` is defined as `true`,
			 * then the hex part of the string is padded up to
			 * `Z_STR_POINTER_CHARS` characters (default is 8).
			 */
			template <typename PTR, typename = typename std::enable_if<std::is_pointer<PTR>::value,PTR>::type>
			string(PTR pointer);

			/**
			 * \brief Construct from floating-point.
			 *
			 * \param value A floating-point number.
			 * \param base The number's base.
			 * \param precision The number of characters after the decimal point.
			 * \param padSize Number of characters to pad up to.
			 *
			 * Converts a floating-point number to a string in the given base.
			 * If the character count is less than the pad size, zeroes
			 * are added to the left side until the character count equals
			 * the pad size. Valid base sizes are from 2 to 36, and anything
			 * else is assumed to be base 10. If the pad size is less than 1,
			 * no character padding is applied. If the precision is 1 or greater,
			 * then exactly that many digits will show after the decimal point,
			 * otherwise up to `Z_STR_FLOAT_PRECISION` digits will show
			 * (default is 6).
			 */
			string(double value, int base = 10, int precision = 0, int padSize = 0);

			/**
			 * \brief Construct from complex number.
			 *
			 * \param value A floating-point complex number.
			 * \param base The number's base.
			 * \param precision The number of characters after the decimal point.
			 *
			 * Converts a floating-point complex number to a string in the given base.
			 * This string will look like `X+Yi`, `X-Yi`, `-X+Yi`, or `-X-Yi`.
			 * Valid base sizes are from 2 to 36, and anything
			 * else is assumed to be base 10. If the precision is 1 or greater,
			 * then exactly that many digits will show after the decimal point,
			 * otherwise up to `Z_STR_FLOAT_PRECISION` digits will show
			 * (default is 6).
			 */
			string(const std::complex<double>& value, int base = 10, int precision = 0);

			/**
			 * \brief Construct from an ASCII string.
			 *
			 * \param other An ASCII encoded string.
			 *
			 * Copies over characters from the given string, converting them
			 * to the appropriate encoding scheme for this string.
			 */
			explicit string(const string<ascii>& other);

			/**
			 * \brief Construct from a UTF-8 string.
			 *
			 * \param other A UTF-8 encoded string.
			 *
			 * Copies over characters from the given string, converting them
			 * to the appropriate encoding scheme for this string.
			 */
			explicit string(const string<utf8>& other);

			/**
			 * \brief Construct from a UTF16 string.
			 *
			 * \param other A UTF16 encoded string.
			 *
			 * Copies over characters from the given string, converting them
			 * to the appropriate encoding scheme for this string.
			 */
			explicit string(const string<utf16>& other);

			/**
			 * \brief Construct from a UTF32 string.
			 *
			 * \param other A UTF32 encoded string.
			 *
			 * Copies over characters from the given string, converting them
			 * to the appropriate encoding scheme for this string.
			 */
			explicit string(const string<utf32>& other);

			/// Lvalue copy-constructor
			string(string&&);

			///Destructor
			~string();

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
			const uint32_t at(size_t index) const;

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
			const uint32_t operator[](size_t index) const;

			/**
			 * \brief Get the size of the string in memory
			 *
			 * \return The number of bytes in memory this string currently consumes.
			 */
			size_t size() const;

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
			size_t length() const;

			/**
			 * \brief Get the individual character count of the string.
			 *
			 * \return The number of characters in the string.
			 *
			 * For UTF-8 strings, multibyte sequences are counted
			 * count as a single character.
			 *
			 * \see length()
			 */
			size_t chars() const;

			/**
			 * \brief Get the single-byte cstring pointer.
			 *
			 * \return A pointer to the string data, if this string is in a
			 * single-byte format. `NULL` otherwise.
			 */
			const uint8_t* cstring() const;

			/**
			 * \brief Get the two-byte cstring pointer.
			 *
			 * \return A pointer to the string data, if this string is in a
			 * two-byte format. `NULL` otherwise.
			 */
			const uint16_t* nstring() const;

			/**
			 * \brief Get the four-byte cstring pointer.
			 *
			 * \return A pointer to the string data, if this string is in a
			 * four-byte format. `NULL` otherwise.
			 */
			const uint32_t* wstring() const;

			/**
			 * \brief Get the encoding of this string.
			 *
			 * \return An integer indicating this string's encoding.
			 *
			 * \see encoding.h
			 */
			constexpr encoding format() const;

			/**
			 * \brief Convert this string to an integer.
			 *
			 * \param base The base this string is being interpreted as.
			 *
			 * \return A signed integer representation of this string, if it
			 * is a valid integer. 0 otherwise.
			 *
			 * \see isInteger()
			 */
			long integer(int base = 10) const;

			/**
			 * \brief Convert this string to a floating-point value.
			 *
			 * \param base The base this string is being interpreted as.
			 *
			 * \return The floating-point representation of this string, if it
			 * is a valid float. 0 otherwise.
			 *
			 * \see isFloating()
			 */
			double floating(int base = 10) const;

			/**
			 * \brief Convert this string to a complex value.
			 *
			 * \param base The base this string is being interpreted as.
			 *
			 * \return The floating-point complex representation of this string,
			 * if it is a valid complex float. 0 otherwise.
			 *
			 * \see isComplex()
			 */
			std::complex<double> complex(int base = 10) const;

			/**
			 * \brief Count the occurrences of a sub-string.
			 *
			 * \param other The sub-string to search for.
			 *
			 * \return The number of times the given sub-string appears in
			 * the full string.
			 */
			int count(const string& other) const;

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
			int find(const string& other, int occurrence = 1) const;

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
			int findLast(const string& other, int occurrence = 1) const;

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
			int findAfter(const string& other, size_t index, int occurrence = 1) const;

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
			int findBefore(const string& other, size_t index, int occurrence = 1) const;

			/**
			 * \brief Check if a sub-string is found at the given index.
			 *
			 * \param other The sub-string to check for.
			 * \param index The index in the string to look at.
			 *
			 * \return True if the sub-string was found beginning at the given index.
			 * False otherwise.
			 */
			bool foundAt(const string& other, size_t index) const;

			/**
			 * \brief Check if a sub-string ends at the given index.
			 *
			 * \param other The sub-string to check for.
			 * \param index The index in the string to look at.
			 *
			 * \return True if the sub-string was found ending at the given index.
			 * False otherwise.
			 */
			bool foundEndAt(const string& other, size_t index) const;

			/**
			 * \brief Check if the string begins with a given sub-string.
			 *
			 * \param other The sub-string to check for.
			 *
			 * \return True if this string begins with the given sub-string.
			 * False otherwise.
			 */
			bool beginsWith(const string& other) const;

			/**
			 * \brief Check if the string ends with a given sub-string.
			 *
			 * \param other The sub-string to check for.
			 *
			 * \return True if this string ends with the given sub-string.
			 * False otherwise.
			 */
			bool endsWith(const string& other) const;

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
			bool isInteger(int base = 10) const;

			/**
			 * \brief Check if this string can convert to a floating-point value.
			 *
			 * \param base The base this string is being interpreted as.
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
			bool isFloating(int base = 10) const;

			/**
			 * \brief Check if this string can convert to a complex value.
			 *
			 * \param base The base this string is being interpreted as.
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
			bool isComplex(int base = 10) const;

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
			string substr(size_t index, int count) const;

			/**
			 * \brief Append another string to the end of this one.
			 *
			 * \param other The string to append.
			 *
			 * \return A reference to this string after the other string is appended.
			 *
			 * \see operator+=()
			 */
			const string& append(const string& other);

			/**
			 * \brief Insert another string into this one.
			 *
			 * \param other The string to insert.
			 * \param index The index to insert before.
			 *
			 * \return A reference to this string after the other string is inserted.
			 *
			 * Inserts the given string before the given index. So, for `B = "abc"` and
			 * `A = "xyz"`, `A.insert(B,0)` would give `"abcxyz"`, `A.insert(B,1)` would
			 * give `"xabcyz"`, etc.
			 */
			const string& insert(const string& other, size_t index);

			/**
			 * \brief Remove occurrences of the given sub-string.
			 *
			 * \param other The sub-string to remove.
			 * \param occurrence The occurrence to remove.
			 *
			 * \return A reference to this string after the sub-string is removed.
			 *
			 * If the occurrence is 0, all occurrences of the sub-string are removed.
			 * If the occurrence is negative, then none are removed. Otherwise, only the
			 * specified occurrence of the sub-string is removed from the main string,
			 * starting from the beginning.
			 */
			const string& remove(const string& other, int occurrence = 0);

			/**
			 * \brief Remove a subset of the string.
			 *
			 * \param index The starting index.
			 * \param count The number of characters to remove.
			 *
			 * \return A reference to this string after the characters are removed.
			 *
			 * If count is 0, no characters are removed. If count is negative, characters are
			 * removed in reverse order.
			 */
			const string& remove(size_t index, int count);

			/**
			 * \brief Replace occurrences of the given sub-string.
			 *
			 * \param findStr The sub-string to replace.
			 * \param replStr The string to replace the sub-string with.
			 * \param occurrence The occurrence to remove.
			 *
			 * \return A reference to this string after the sub-string is replaced.
			 *
			 * If the occurrence is 0, all occurrences of the sub-string are replaced.
			 * If the occurrence is negative, then none are replaced. Otherwise, only the
			 * specified occurrence of the sub-string is replaced, starting from the
			 * beginning of this string.
			 */
			const string& replace(const string& findStr, const string& replStr, int occurrence = 0);

			/**
			 * \brief Replace a subset of the string.
			 *
			 * \param index The starting index.
			 * \param count The number of characters to remove.
			 * \param other The string to replace the subset with.
			 *
			 * \return A reference to this string after the characters are replaced.
			 *
			 * If count is 0, no characters are replaced. Characters are replaced in forward-order
			 * whether count is positive or negative.
			 */
			const string& replace(size_t index, int count, const string& other);

			/**
			 * \brief Left-pad this string up to a given chracter count.
			 *
			 * \param other The string to pad on the left of this string.
			 * \param padSize the number of characters to pad up to.
			 *
			 * \return A reference to this string after padding.
			 *
			 * If the given string contains more than one character, then some of
			 * that string's characters may be cut off in the resultant string. If
			 * the pad size is equal or less than the current chracter count, then no
			 * padding will be added.
			 */
			const string& padLeft(const string& other, size_t padSize);

			/**
			 * \brief Right-pad this string up to a given chracter count.
			 *
			 * \param other The string to pad on the left of this string.
			 * \param padSize the number of characters to pad up to.
			 *
			 * \return A reference to this string after padding.
			 *
			 * If the given string contains more than one character, then some of
			 * that string's characters may be cut off in the resultant string. If
			 * the pad size is equal or less than the current chracter count, then no
			 * padding will be added.
			 */
			const string& padRight(const string& other, size_t padSize);

			/**
			 * \brief Remove padding from the left side of this string.
			 *
			 * \param other The pad string to remove.
			 *
			 * \return A reference to this string after removing left-padding.
			 *
			 * Removes all occurrences of the given pad string from the left
			 * side of this string.
			 */
			const string& unPadLeft(const string& other);

			/**
			 * \brief Remove padding from the right side of this string.
			 *
			 * \param other The pad string to remove.
			 *
			 * \return A reference to this string after removing right-padding.
			 *
			 * Removes all occurrences of the given pad string from the right
			 * side of this string.
			 */
			const string& unPadRight(const string& other);

			/**
			 * \brief Remove padding from the both sides of this string.
			 *
			 * \param other The pad string to remove.
			 *
			 * \return A reference to this string after removing padding.
			 *
			 * Removes all occurrences of the given pad string from the left
			 * and right sides of this string.
			 */
			const string& unPad(const string& other);

			/**
			 * \brief Remove all sequential duplicates from this string.
			 *
			 * \param other The string to remove duplicates of.
			 *
			 * \return A reference to this string after removing duplicates.
			 *
			 * Removes all contiguous repetitions of the given sub-string, leaving
			 * one instance of the sub-string where the group was.
			 */
			const string& cutDuplicates(const string& other);

			/**
			 * \brief Concatenate two strings.
			 *
			 * \param other The string to append.
			 *
			 * \return The concatenation of this and the given string.
			 *
			 * Appends the given string to the end of this string's characters and
			 * returns the result. This string is not modified.
			 */
			string operator+(const string& other) const;

			/**
			 * \brief Append another string to the end of this one.
			 *
			 * \param other The string to append.
			 *
			 * \return A reference to this string after the other string is appended.
			 *
			 * \see append()
			 */
			const string& operator+=(const string& other);

			/**
			 * \brief Assign the contents of this string.
			 *
			 * \param other The string to copy from.
			 *
			 * \return A reference to this string after assignment.
			 *
			 * Copies all characters from the given string over to this string,
			 * replacing current data.
			 */
			const string& operator=(const string& other);

			/**
			 * \brief Equality comparison.
			 *
			 * \param other The string to compare to.
			 *
			 * \return True if the strings exactly match. False otherwise.
			 */
			bool operator==(const string& other) const;

			/**
			 * \brief Inequality comparison.
			 *
			 * \param other The string to compare to.
			 *
			 * \return True if the strings do not exactly match. False otherwise.
			 */
			bool operator!=(const string& other) const;

			/**
			 * \brief Greater-than comparison.
			 *
			 * \param other The string to compare to.
			 *
			 * \return True if this string comes after the given string alphabetically. False otherwise.
			 */
			bool operator>(const string& other) const;

			/**
			 * \brief Greater-than or equal comparison.
			 *
			 * \param other The string to compare to.
			 *
			 * \return True if this string does not come before the given string alphabetically. False otherwise.
			 */
			bool operator>=(const string& other) const;

			/**
			 * \brief Less-than comparison.
			 *
			 * \param other The string to compare to.
			 *
			 * \return True if this string comes before the given string alphabetically. False otherwise.
			 */
			bool operator<(const string& other) const;

			/**
			 * \brief Less-than or equal comparison.
			 *
			 * \param other The string to compare to.
			 *
			 * \return True if this string does not come after the given string alphabetically. False otherwise.
			 */
			bool operator<=(const string& other) const;

			void serialIn(inputStream&);
			void serialOut(outputStream&) const;

			void read(inputStream&, uint32_t delim = 0);
			void readln(inputStream&);
			void write(outputStream&) const;
			void writeln(outputStream&) const;
		};

		#include "string/default.h"
		#include "string/utf32.h"
		#include "string/utf16.h"
		#include "string/ascii.h"
		#include "string/utf8.h"
	}
}


static size_t integralBuf(unsigned long integral, int base, uint8_t* buf)
{
	if (integral)
	{
		size_t length = 0;

		while (integral)
		{
			buf[length] = z::core::numeral(integral%base);
			integral /= base;
			length++;
		}

		return length;
	}
	else
	{
		buf[0] = '0';
		return 1;
	}
}

static size_t fractionalBuf(double fractional, int base, int precision, bool force, uint8_t* buf)
{
	if (fractional)
	{
		fractional -= (double)(long)fractional;
		// return 0;
		size_t length = 0;
		double mult = base;
		int i = 0;

		bool cont = true;
		while ((i < precision) && cont)
		{
			fractional *= mult;

			uint8_t chr = z::core::numeral((int)fractional);
			fractional -= (double)(long)fractional;

			buf[i] = chr;

			i++;
			if (chr != '0') length = i;
			if (!fractional && !force) cont = false;
		}

		return length;
	}
	else
	{
		return 0;
	}
}
