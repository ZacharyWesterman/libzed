#pragma once

#include <complex>
#include <cstring>
#include <functional>
#include <initializer_list>
#include <type_traits>
#include <utility>

#include <istream>
#include <ostream>

#include "../encoding.hpp"
#include "charFunctions.hpp"
#include "zstr.hpp"

#include "arrayLike.hpp"
#include "sizable.hpp"

#include "stringIterator.hpp"

#ifdef __has_include
#if __has_include(<cereal/cereal.hpp>)
#include <cereal/types/string.hpp>
#endif
#endif

namespace z {
namespace core {
/**
 * \brief A template class for character strings.
 *
 * This class focuses on how characters are encoded rather than character size.
 * Possible encoding schemes are ASCII, UTF-8, UTF16, and UTF32.<br/>
 * When characters are input or output, they are converted to or from their
 * encoding scheme to a single UTF32 character. Thus, the "default" character
 * type is `uint32_t`.
 *
 * Note this class is compatible with standard library streams, but also has
 * read(), readln(), write(), writeln() methods for interacting with core::stream
 * objects (Useful if a specific encoding is strictly required).
 *
 * To keep string types short, you may append _u32, _u16, _u8, or _asc after any literals.
 * For example, here are a few different ways to construct a UTF32 encoded string:
 * ```
 * auto str1 = "hello world"_u32;
 * auto str2 = L"hello too!"_u32;
 * auto str3 = 'X'_u32;
 * auto str4 = 1234_u32;
 * ```
 * The above will all be of type z::core::string<z::utf32>
 *
 * \note Allocated memory is increased as needed with approximate 1.5x growth, and
 * is not decreased on subsequent data changes, except in the case where
 * data is copied over to a different string.
 *
 * \see encoding.h
 * \see zstr.h
 */
template <encoding E = utf8> class string : public sizable, public arrayLike<uint32_t, stringIterator<E>> {
	friend string<ascii>;
	friend string<utf8>;
	friend string<utf16>;
	friend string<utf32>;

private:
	uint8_t *data;
	int data_len;
	int character_ct;

	void initChar(uint32_t, int) noexcept;
	int charSize() const noexcept;

	void initInt(long long, int, int) noexcept;
	void initFloat(double, int, int, bool, int) noexcept;
	void initPointer(void *) noexcept;
	void initComplex(const std::complex<double> &, int, int, bool, int) noexcept;

public:
	/// Default string constructor
	string() noexcept;

	/**
	 * \brief Construct string from a single-byte character.
	 *
	 * \param chr Initializing character.
	 *
	 * Character is assumed to be compatible with this string's encoding.
	 */
	explicit string(char chr) noexcept;

	/**
	 * \brief Construct string from a wide character.
	 *
	 * \param chr Initializing character.
	 *
	 * Converts the given character to the appropriate encoding for this string.
	 */
	explicit string(wchar_t chr) noexcept;

	/**
	 * \brief Construct string from uint32_t.
	 *
	 * \param chr Initializing character.
	 *
	 * This constructor exists to allow a "default" string character to
	 * again be constructed into a string.
	 * Converts the given character to the appropriate encoding for this string.
	 */
	string(const uint32_t &chr) noexcept;

	/**
	 * \brief Construct from a cstring of single-byte characters.
	 *
	 * \param str Null-terminated cstring.
	 *
	 * All characters are assumed to be compatible with this string's encoding.
	 */
	string(const char *str) noexcept;

	/**
	 * \brief Construct from a cstring of single-byte characters, with a known length.
	 *
	 * \param str A cstring, which may or may not be null-terminated.
	 * \param len The length of the string.
	 *
	 * All characters are assumed to be compatible with this string's encoding.
	 */
	string(const char *str, size_t len) noexcept;

	/**
	 * \brief Construct from a cstring of wide characters.
	 *
	 * \param str Null-terminated cstring.
	 *
	 * Converts the characters in the given cstring to the appropriate encoding
	 * for this string.
	 */
	string(const wchar_t *str) noexcept;

	/**
	 * \brief Construct from a cstring of wide characters, with a known length.
	 *
	 * \param str A cstring, which may or may not be null-terminated.
	 * \param len The length of the string.
	 *
	 * All characters are assumed to be compatible with this string's encoding.
	 */
	string(const wchar_t *str, size_t len) noexcept;

	/**
	 * \brief Construct from a std::string.
	 *
	 * \param str The string to copy.
	 *
	 * All characters are assumed to be compatible with this string's encoding.
	 */
	string(const std::string &str) noexcept : string(str.c_str(), str.size()) {}

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
	template <typename INT, typename = typename std::enable_if<std::is_integral<INT>::value && !std::is_same<INT, char>::value &&
																																 !std::is_same<INT, wchar_t>::value,
																														 INT>::type>
	string(INT value, int base = 10, int padSize = 0) noexcept {
		this->initInt((long long)value, base, padSize);
	}

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
	template <typename PTR, typename = typename std::enable_if<std::is_pointer<PTR>::value && !std::is_same<PTR, char *>::value &&
																																 !std::is_same<PTR, wchar_t *>::value,
																														 PTR>::type>
	string(PTR pointer) noexcept {
		this->initPointer((void *)pointer);
	}

	/**
	 * \brief Construct from floating-point.
	 *
	 * \param value A floating-point number.
	 * \param base The number's base.
	 * \param precision The number of characters after the decimal point.
	 * \param scientific Whether to use scientific notation.
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
	template <typename FLT, typename = typename std::enable_if<std::is_floating_point<FLT>::value, FLT>::type>
	string(FLT value, int base = 10, int precision = 0, bool scientific = true, int padSize = 0) noexcept {
		this->initFloat((double)value, base, precision, scientific, padSize);
	}

	/**
	 * \brief Construct from complex number.
	 *
	 * \param value A floating-point complex number.
	 * \param base The number's base.
	 * \param precision The number of characters after the decimal point.
	 * \param scientific Whether to use scientific notation.
	 * \param padSize Number of characters to pad up to.
	 *
	 * Converts a floating-point complex number to a string in the given base.
	 * This string will look like `X+Yi`, `X-Yi`, `-X+Yi`, or `-X-Yi`.
	 * Valid base sizes are from 2 to 36, and anything
	 * else is assumed to be base 10. If the precision is 1 or greater,
	 * then exactly that many digits will show after the decimal point,
	 * otherwise up to `Z_STR_FLOAT_PRECISION` digits will show
	 * (default is 6).
	 */
	template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
	string(const std::complex<T> &value, int base = 10, int precision = 0, bool scientific = true, int padSize = 0) noexcept {
		this->initComplex((std::complex<double>)value, base, precision, scientific, padSize);
	}

	/**
	 * \brief Construct from an ASCII string.
	 *
	 * \param other An ASCII encoded string.
	 *
	 * Copies over characters from the given string, converting them
	 * to the appropriate encoding scheme for this string.
	 */
	string(const string<ascii> &other) noexcept;

	/**
	 * \brief Construct from a UTF-8 string.
	 *
	 * \param other A UTF-8 encoded string.
	 *
	 * Copies over characters from the given string, converting them
	 * to the appropriate encoding scheme for this string.
	 */
	string(const string<utf8> &other) noexcept;

	/**
	 * \brief Construct from a UTF16 string.
	 *
	 * \param other A UTF16 encoded string.
	 *
	 * Copies over characters from the given string, converting them
	 * to the appropriate encoding scheme for this string.
	 */
	string(const string<utf16> &other) noexcept;

	/**
	 * \brief Construct from a UTF32 string.
	 *
	 * \param other A UTF32 encoded string.
	 *
	 * Copies over characters from the given string, converting them
	 * to the appropriate encoding scheme for this string.
	 */
	string(const string<utf32> &other) noexcept;

	/**
	 * \brief Move constructor.
	 *
	 * Moves a string of the same encoding. Just does a mem copy.
	 *
	 * \param other The string to move.
	 */
	string(string &&other) noexcept {
		data = other.data;
		data_len = other.data_len;
		character_ct = other.character_ct;
		other.data = NULL;
	}

	/**
	 * \brief Move assignment operator.
	 *
	 * Moves a string of the same encoding. Since this string has already been
	 * allocated data, we must first deallocate, then do a mem copy.
	 *
	 * \param other The string to move.
	 */
	string &operator=(string &&other) noexcept {
		if (&other != this) {
			delete[] data;
			data = other.data;
			data_len = other.data_len;
			character_ct = other.character_ct;
			other.data = NULL;
		}

		return *this;
	}

	/// Destructor
	~string() noexcept {
		if (data) {
			delete[] data;
		}
	}

	/**
	 * \brief Increase the space allocated for this string.
	 *
	 * If this string currently has fewer than max_chars allocated,
	 * enough space is reallocated to hold at least that many characters.
	 * Note that strings follow an approximate 1.5x growth pattern.
	 *
	 * \param charCount The minimum number of characters this string should be able to contain before reallocating.
	 */
	void increase(int charCount) noexcept;

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
	uint32_t at(int index) const noexcept override;

	/**
	 * \brief Get the size of the string in memory
	 *
	 * \return The number of bytes in memory this string currently consumes.
	 */
	size_t size() const noexcept override;

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
	int length() const noexcept override;

	/**
	 * \brief Get the individual character count of the string.
	 *
	 * \return The number of characters in the string.
	 *
	 * For UTF-8 strings, multibyte sequences are counted
	 * as a single character.
	 *
	 * \see length()
	 */
	int chars() const noexcept;

	/**
	 * \brief Get the single-byte cstring pointer.
	 *
	 * \return A pointer to the string data, if this string is in a
	 * single-byte format. `NULL` otherwise.
	 */
	const char *cstring() const noexcept;

	/**
	 * \brief Get the two-byte cstring pointer.
	 *
	 * \return A pointer to the string data, if this string is in a
	 * two-byte format. `NULL` otherwise.
	 */
	const uint16_t *nstring() const noexcept;

	/**
	 * \brief Get the four-byte cstring pointer.
	 *
	 * \return A pointer to the string data, if this string is in a
	 * four-byte format. `NULL` otherwise.
	 */
	const uint32_t *wstring() const noexcept;

	/**
	 * \brief Get the encoding of this string.
	 *
	 * \return An integer indicating this string's encoding.
	 *
	 * \see encoding.h
	 */
	constexpr encoding format() const noexcept {
		return E;
	}

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
	 * \brief Convert this string to an integer.
	 * \see integer()
	 */
	template <typename INT, typename = typename std::enable_if<std::is_integral<INT>::value, INT>::type>
	explicit operator INT() const noexcept {
		return integer();
	}

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
	 * \brief Convert this string to a floating-point value.
	 * \see floating()
	 */
	explicit operator double() const noexcept {
		return floating();
	}

	/**
	 * \brief Convert this string to a floating-point value.
	 * \see floating()
	 */
	explicit operator float() const noexcept {
		return floating();
	}

	/**
	 * \brief Check if this string contains at least one character.
	 * \return True if `length() > 0`, false otherwise.
	 */
	constexpr explicit operator bool() const noexcept {
		return length() > 0;
	}

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
	 * \brief Count the occurrences of a sub-string.
	 *
	 * \param other The sub-string to search for.
	 *
	 * \return The number of times the given sub-string appears in
	 * the full string.
	 */
	int count(const string &other) const noexcept;

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
	int find(const string &other, int occurrence = 1) const noexcept {
		return this->findAfter(other, 0, occurrence);
	}

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
	int found(const string &other, int occurrence = 1) const noexcept {
		return this->findAfter(other, 0, occurrence) >= 0;
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
	int findLast(const string &other, int occurrence = 1) const noexcept {
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
	int findAfter(const string &other, int index, int occurrence = 1) const noexcept {
		(void)occurrence;
		for (int i = index; i < character_ct; i++) {
			if (foundAt(other, i)) {
				return i;
			}
		}

		return -1;
	}

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
	int findBefore(const string &other, int index, int occurrence = 1) const noexcept;

	/**
	 * \brief Check if a sub-string is found at the given index.
	 *
	 * \param other The sub-string to check for.
	 * \param index The index in the string to look at.
	 *
	 * \return True if the sub-string was found beginning at the given index.
	 * False otherwise.
	 */
	bool foundAt(const string &other, int index) const noexcept;

	/**
	 * \brief Check if a sub-string ends at the given index.
	 *
	 * \param other The sub-string to check for.
	 * \param index The index in the string to look at.
	 *
	 * \return True if the sub-string was found ending at the given index.
	 * False otherwise.
	 */
	bool foundEndAt(const string &other, int index) const noexcept;

	/**
	 * \brief Check if the string begins with a given sub-string.
	 *
	 * \param other The sub-string to check for.
	 *
	 * \return True if this string begins with the given sub-string.
	 * False otherwise.
	 */
	bool beginsWith(const string &other) const noexcept {
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
	bool endsWith(const string &other) const noexcept {
		return this->foundEndAt(other, character_ct - 1);
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
	bool isInteger(int base = 10) const noexcept {
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
	bool isFloating(int base = 10, uint32_t decimal = '.') const noexcept {
		return type(base, decimal) & (zstr::integer | zstr::floating);
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
	bool isComplex(int base = 10, uint32_t decimal = '.') const noexcept {
		return type(base, decimal) & (zstr::integer | zstr::floating | zstr::complex);
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
	string substr(int index, int count) const noexcept;

	/**
	 * \brief Append another string to the end of this one.
	 *
	 * \param other The string to append.
	 *
	 * \return A reference to this string after the other string is appended.
	 *
	 * \see operator+=()
	 */
	string &append(const string &other) noexcept {
		return operator+=(other);
	}

	/**
	 * \brief Append a single character to the end of the string.
	 *
	 * Use this method if you need to append raw characters (i.e. they're coming from a stream),
	 * as it is more efficient than first casting the character to a string and appending that way.
	 *
	 * \param chr The character to append, in UTF32 format.
	 *
	 * \return A reference to this string after the character is appended.
	 *
	 * \see operator+=()
	 */
	string &append(uint32_t chr) noexcept;

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
	string &insert(const string &other, int index) noexcept;

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
	string &remove(const string &other, int occurrence = 0) noexcept;

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
	string &remove(int index, int count) noexcept;

	/**
	 * \brief Remove all characters from a given index to the end of the string.
	 *
	 * This function is faster than remove() at truncating string contents,
	 * because no actual data copying takes place. The character at the given
	 * index is simply set to the null char \\0.
	 *
	 * \param index The index of the first character to remove.
	 *
	 * \return A reference to this string after the characters are truncated.
	 */
	string &truncate(int index) noexcept;

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
	string &replace(const string &findStr, const string &replStr, int occurrence = 0) noexcept;

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
	string &replace(int index, int count, const string &other) noexcept;

	/**
	 * \brief Copy this string, left-padded given character count.
	 *
	 * \param other The string to pad on the left of the string.
	 * \param padSize the number of characters to pad up to.
	 *
	 * \return A copy of this string after padding.
	 *
	 * If the given string contains more than one character, then some of
	 * that string's characters may be cut off in the resultant string. If
	 * the pad size is equal or less than the current chracter count, then no
	 * padding will be added.
	 */
	string padLeft(const string &other, int padSize) const noexcept {
		string newString = *this;
		return newString.padLeftIn(other, padSize);
	}

	/**
	 * \brief Left-pad this string up to a given character count.
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
	string &padLeftIn(const string &other, int padSize) noexcept;

	/**
	 * \brief Copy this string, right-padded given character count.
	 *
	 * \param other The string to pad on the right of the string.
	 * \param padSize the number of characters to pad up to.
	 *
	 * \return A copy of this string after padding.
	 *
	 * If the given string contains more than one character, then some of
	 * that string's characters may be cut off in the resultant string. If
	 * the pad size is equal or less than the current chracter count, then no
	 * padding will be added.
	 */
	string padRight(const string &other, int padSize) const noexcept {
		string newString = *this;
		return newString.padRightIn(other, padSize);
	}

	/**
	 * \brief Right-pad this string up to a given character count.
	 *
	 * \param other The string to pad on the right of this string.
	 * \param padSize the number of characters to pad up to.
	 *
	 * \return A reference to this string after padding.
	 *
	 * If the given string contains more than one character, then some of
	 * that string's characters may be cut off in the resultant string. If
	 * the pad size is equal or less than the current chracter count, then no
	 * padding will be added.
	 */
	string &padRightIn(const string &other, int padSize) noexcept;

	/**
	 * \brief Repeat this string a specific number of times.
	 *
	 * \param count the number of times to repeat this string.
	 *
	 * \return Resultant string.
	 */
	string repeat(int count) const noexcept {
		string value;
		value.increase(character_ct * count);
		for (int i = 0; i < count; ++i) {
			value.append(*this);
		}
		return value;
	}

	/**
	 * \brief Copies this string and removes padding from the left side of the result.
	 *
	 * \param other The pad string to remove.
	 *
	 * \return A copy of this string with left-padding removed.
	 *
	 * Removes all occurrences of the given pad string from the left
	 * side of this string.
	 */
	string trimLeft(const string &other = "") const noexcept {
		if ((character_ct < other.character_ct) || !character_ct) {
			return *this;
		}

		int index = 0;
		if (other.length()) {
			while (foundAt(other, index)) {
				index += other.character_ct;
			}
		} else {
			while (isWhiteSpace(at(index))) {
				++index;
			}
		}

		return substr(index, character_ct - index);
	}

	/**
	 * \brief Copies this string and removes padding from the right side of the result.
	 *
	 * \param other The pad string to remove.
	 *
	 * \return A copy of this string with right-padding removed.
	 *
	 * Removes all occurrences of the given pad string from the right
	 * side of this string.
	 */
	string trimRight(const string &other = "") const noexcept {
		if ((character_ct < other.character_ct) || !character_ct) {
			return *this;
		}

		int index = character_ct - 1;
		if (other.length()) {
			while (foundEndAt(other, index)) {
				index -= other.character_ct;
			}
		} else {
			while (isWhiteSpace(at(index))) {
				--index;
			}
		}

		return substr(0, index);
	}

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
	string &trimLeftIn(const string &other = "") noexcept {
		if ((character_ct < other.character_ct) || !character_ct) {
			return *this;
		}

		int index = 0;
		if (other.length()) {
			while (foundAt(other, index)) {
				index += other.character_ct;
			}
		} else {
			while (isWhiteSpace(this->at(index))) {
				++index;
			}
		}

		return remove(0, index);
	}

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
	string &trimRightIn(const string &other = "") noexcept {
		if ((character_ct < other.character_ct) || !character_ct) {
			return *this;
		}

		int index = character_ct - 1;
		if (other.length()) {
			while (foundEndAt(other, index)) {
				index -= other.character_ct;
			}
		} else {
			while (isWhiteSpace(at(index))) {
				--index;
			}
		}

		return remove(index + 1, character_ct);
	}

	/**
	 * \brief Copies this string and removes padding from both sides of the result.
	 *
	 * \param other The pad string to remove.
	 *
	 * \return A copy of this string with right-padding removed.
	 *
	 * Removes all occurrences of the given pad string from the left
	 * and right sides of this string.
	 */
	string trim(const string &other = "") const noexcept {
		auto result = trimLeft(other);
		return result.trimRightIn(other);
	}

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
	string &trimIn(const string &other = "") noexcept {
		trimLeftIn(other);
		return trimRightIn(other);
	}

	/**
	 * \brief Reset to a null string.
	 */
	void clear() noexcept;

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
	string &cutDuplicates(const string &other) noexcept;

	/**
	 * \brief Get an uppercase version of this string.
	 *
	 * \return A duplicate of this string, converted to uppercase.
	 *
	 * \see toUpper()
	 */
	string upper() const noexcept {
		return string(*this).toUpper();
	}

	/**
	 * \brief Get a lowercase version of this string.
	 *
	 * \return A duplicate of this string, converted to lowercase.
	 *
	 * \see toLower()
	 */
	string lower() const noexcept {
		return string(*this).toLower();
	}

	/**
	 * \brief Get a camelcase version of this string.
	 *
	 * \return A duplicate of this string, converted to camelcase.
	 *
	 * \see toCamel()
	 */
	string camel() const noexcept {
		return string(*this).toCamel();
	}

	/**
	 * \brief Convert all characters in the string to uppercase.
	 *
	 * \return A reference to this string after converting to uppercase.
	 *
	 * \see upper()
	 */
	string &toUpper() noexcept;

	/**
	 * \brief Convert all characters in the string to lowercase.
	 *
	 * \return A reference to this string after converting to lowercase.
	 *
	 * \see lower()
	 */
	string &toLower() noexcept;

	/**
	 * \brief Convert all characters in the string to camelcase.
	 *
	 * \return A reference to this string after converting to camelcase.
	 *
	 * \see camel()
	 */
	string &toCamel() noexcept;

	/**
	 * \brief Filter out all characters not in the given range.
	 *
	 * example: this->filter('A', 'Z') will remove all characters not in the
	 * range A -> Z, inclusive.
	 *
	 * \param first The first character in the range.
	 * \param last The last character in the range.
	 * \param invert Invert the selection, that is, remove all characters that <B>ARE</B> in the given range.
	 *
	 * \return A duplicate of this string with all non-matching characters removed.
	 */
	string filter(uint32_t first, uint32_t last, bool invert = false) const noexcept {
		return filter({first, last}, invert);
	}

	/**
	 * \brief Filter out all characters not in the given range.
	 *
	 * example: this->filter({'A', 'Z'}) will remove all characters not in the
	 * range A -> Z, inclusive.
	 *
	 * \param range A std::pair denoting the character range.
	 * \param invert Invert the selection, that is, remove all characters that <B>ARE</B> in the given range.
	 *
	 * \return A duplicate of this string with all non-matching characters removed.
	 */
	string filter(const std::pair<uint32_t, uint32_t> &range, bool invert = false) const noexcept {
		return filter({range}, invert);
	}

	/**
	 * \brief Filter out all characters not in the given range.
	 *
	 * example: this->filter({{'A', 'Z'}, {'a', 'z'}, {'0', '9'}}) will remove all characters that are not
	 * alphanumeric.
	 *
	 * \param list A list of std::pair objects denoting all valid character ranges.
	 * \param invert Invert the selection, that is, remove all characters that <B>ARE</B> in the given range.
	 *
	 * \return A duplicate of this string with all non-matching characters removed.
	 */
	string filter(const std::initializer_list<const std::pair<uint32_t, uint32_t>> &list, bool invert = false) const noexcept;

	/**
	 * \brief Filter out all characters not in the given string.
	 *
	 * example: this->filter("aeiouyAEIOUY") will remove all non-vowel characters.
	 *
	 * \param list A string indicating the list of valid characters.
	 * \param invert Invert the selection, that is, remove all characters that <B>ARE</B> in the given range.
	 *
	 * \return A duplicate of this string with all non-matching characters removed.
	 */
	string filter(const string &list, bool invert = false) const noexcept;

	/**
	 * \brief Filter out characters based on a function.
	 *
	 * example: `my_string.filter(z::core::isAlphaNumeric)` will remove all characters that are not alphanumeric.
	 *
	 * \param lambda The function used to determine whether to keep a given character. If this function returns true, the
	 * character stays in the string. If false, the character is removed.
	 *
	 * \return A duplicate of this string with all non-matching characters removed.
	 */
	string filter(std::function<bool(uint32_t)> lambda) const noexcept;

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
	bool contains(uint32_t first, uint32_t last, bool exclusive = false) const noexcept {
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
	bool contains(const std::pair<uint32_t, uint32_t> &range, bool exclusive = false) const noexcept {
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
	bool contains(const std::initializer_list<const std::pair<uint32_t, uint32_t>> &list, bool exclusive = false) const noexcept;

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
	bool contains(const string &list, bool exclusive = false) const noexcept;

	/**
	 * \brief Convert this string from one set of characters to another.
	 *
	 * For each character in this string, if it's one of the keys, it will be replaced with the respective value.
	 * For example `zstring("message").cipher("aegms","12345")` will output `4255132`.
	 *
	 * \param keys The characters to find and convert.
	 * \param values The characters to convert to.
	 *
	 * \return A string with all the specified characters replaced.
	 */
	string cipher(const string &keys, const string &values) const noexcept;

	/**
	 * \brief Convert this string from one set of characters to another.
	 *
	 * For each character in this string, it is passed to a function to mutate it.
	 * The function is expected to be of the form `uint32_t func(uint32_t ch) {...}`.
	 * For example `zstring("message").cipher([](auto a){ return a + 1; })` will output `nfttbhf`.
	 *
	 * \param lambda An arbitrary function that converts the characters in this string.
	 *
	 * \return A string with all characters converted according to the lambda.
	 */
	string cipher(std::function<uint32_t(uint32_t)> lambda) const noexcept;

	/**
	 * \brief Generate an English representation of a given number.
	 *
	 * \param value The number to represent in English.
	 * \param ordinal If true, output with ordinal suffix.
	 * \return A string of English words representing the number's value.
	 */
	static string words(long long value, bool ordinal = false) noexcept;

	/**
	 * \brief Get the ordinal suffix of a given integer.
	 *
	 * This gives the suffix only. E.g. ordinal(1) -> "st", ordinal(3) -> "rd", ordinal(57) -> "th", etc.
	 *
	 * \param value The value get the ordinal suffix of.
	 * \return A 2-character string representing the ordinal suffix of the given number.
	 */
	static string ordinal(long long value) noexcept;

	/**
	 * \brief Concatenate two strings.
	 *
	 * \param other The string to append.
	 *
	 * \return The concatenation of this and the given string.
	 *
	 * Appends the given string to the end of this string's characters and
	 * returns the result. This string is not modified.
	 *
	 * \see operator+()
	 */
	string concat(const string &other) const noexcept {
		return *this + other;
	}

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
	string operator+(const string &other) const noexcept;

	/**
	 * \brief Append another string to the end of this one.
	 *
	 * \param other The string to append.
	 *
	 * \return A reference to this string after the other string is appended.
	 *
	 * \see append()
	 */
	string &operator+=(const string &other) noexcept;

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
	string &operator=(const string &other) noexcept;

	/**
	 * \brief Equality comparison.
	 *
	 * \param other The string to compare to.
	 *
	 * \return True if the strings exactly match. False otherwise.
	 */
	bool operator==(const string &other) const noexcept;

	/**
	 * \brief Inequality comparison.
	 *
	 * \param other The string to compare to.
	 *
	 * \return True if the strings do not exactly match. False otherwise.
	 */
	bool operator!=(const string &other) const noexcept {
		return !operator==(other);
	}

	/**
	 * \brief Greater-than comparison.
	 *
	 * \param other The string to compare to.
	 *
	 * \return True if this string comes after the given string alphabetically. False otherwise.
	 */
	bool operator>(const string &other) const noexcept;

	/**
	 * \brief Greater-than or equal comparison.
	 *
	 * \param other The string to compare to.
	 *
	 * \return True if this string does not come before the given string alphabetically. False otherwise.
	 */
	bool operator>=(const string &other) const noexcept {
		return !operator<(other);
	}

	/**
	 * \brief Less-than comparison.
	 *
	 * \param other The string to compare to.
	 *
	 * \return True if this string comes before the given string alphabetically. False otherwise.
	 */
	bool operator<(const string &other) const noexcept;

	/**
	 * \brief Less-than or equal comparison.
	 *
	 * \param other The string to compare to.
	 *
	 * \return True if this string does not come after the given string alphabetically. False otherwise.
	 */
	bool operator<=(const string &other) const noexcept {
		return !operator>(other);
	}

	/**
	 * \brief Read string data from a stream until the given delimiter is encountered.
	 *
	 * Reads until the delimiting character is encountered, or the stream is exhausted.
	 * If the delimiter is 0 or not given, the delimiter is assumed to be any white space character.
	 * Leading delimiter characters are skipped over until a non-delimiter character is encountered or the stream is exhausted.
	 * Any data this string contains is wiped when this function is called.
	 *
	 * \param stream The stream to read from.
	 * \param delim The delimiter to read until.
	 *
	 * \return A reference to this string after reading.
	 */
	string &read(std::istream &stream, uint32_t delim = 0) noexcept;

	/**
	 * \brief Read string data from a stream until a newline is encountered.
	 *
	 * Reads until a newline is encountered (either `\r\n` or `\n`), or the stream is exhausted.
	 * Leading newlines are not skipped, so as to allow for empty lines.
	 * Any data this string contains is wiped when this function is called.
	 *
	 * \param stream The stream to read from.
	 *
	 * \return A reference to this string after reading the line.
	 */
	string &readln(std::istream &stream) noexcept;

	// Read an entire stream's contents into this string.
	string &readall(std::istream &stream) noexcept;

	/**
	 * \brief Write string data to a stream in that stream's encoding.
	 *
	 * \param stream The stream to write to.
	 */
	void write(std::ostream &stream) const noexcept {
		stream << *this;
	}

	/**
	 * \brief Write string data to a stream in its format, appending a newline.
	 *
	 * Actual characters in the newline depends on operating system (usually `\n`, `\r\n` on Windows).
	 *
	 * \param stream The stream to write to.
	 */
	void writeln(std::ostream &stream) const noexcept {
		stream << *this << std::endl;
	}

	/**
	 * \brief Get an iterator for the beginning of the string.
	 *
	 * This member function should not be used directly.
	 * It is meant for C++11's range-based for loop syntax.
	 *
	 * \return An iterator on the first character in the string.
	 */
	stringIterator<E> begin() const noexcept override {
		return stringIterator<E>(data, 0);
	}

	/**
	 * \brief Get an iterator for the end of the string.
	 *
	 * This member function should not be used directly.
	 * It is meant for C++11's range-based for loop syntax.
	 *
	 * \return An iterator after the last character in the string.
	 */
	stringIterator<E> end() const noexcept override {
		return stringIterator<E>(data, character_ct);
	}

#ifdef __has_include
#if __has_include(<cereal/cereal.hpp>)

	/**
	 * \brief Serialization output.
	 * \param ar The output archive.
	 * \return The string to output to serial archive.
	 */
	template <class archive> std::string save_minimal(archive &ar) const {
		(void)ar;
		return string<utf8>(*this).cstring();
	}

	/**
	 * \brief Serialization input.
	 * \param ar The input archive.
	 * \param value The string to input from serial archive.
	 */
	template <class archive> void load_minimal(archive const &ar, std::string const &value) {
		(void)ar;
		operator=(value.c_str());
	}
#endif
#endif

	/**
	 * \brief Stream output operator.
	 * \param ostr The output stream.
	 * \param str The string to write to the stream.
	 * \return A reference to the stream after output.
	 */
	friend std::ostream &operator<<(std::ostream &ostr, const z::core::string<E> &str) {
		return ostr << string<utf8>(str).cstring();
	}

	/**
	 * \brief Stream input operator.
	 * \param istr The input stream.
	 * \param str The string to read from the stream.
	 * \return A reference to the stream after input.
	 */
	friend std::istream &operator>>(std::istream &istr, z::core::string<E> &str) {
		std::string s;
		istr >> s;
		str = s.c_str(); // not efficient to cast strings back & forth, but it works for now.
		return istr;
	}

	/**
	 * \brief Convert to a std::string.
	 * \return A std::string representation, always in UTF-8.
	 */
	std::string str() const noexcept {
		return string<utf8>(*this).cstring();
	}
};
} // namespace core
} // namespace z

typedef z::core::string<> zstring;
typedef z::core::string<z::utf8> zpath;

// Custom literals for simple string construction
z::core::string<z::utf32> operator"" _u32(char value);
z::core::string<z::utf32> operator"" _u32(wchar_t value);
z::core::string<z::utf32> operator"" _u32(const char *value);
z::core::string<z::utf32> operator"" _u32(const char *value, size_t);
z::core::string<z::utf32> operator"" _u32(const wchar_t * value, size_t);

z::core::string<z::utf16> operator"" _u16(char value);
z::core::string<z::utf16> operator"" _u16(wchar_t value);
z::core::string<z::utf16> operator"" _u16(const char *value);
z::core::string<z::utf16> operator"" _u16(const char *value, size_t);
z::core::string<z::utf16> operator"" _u16(const wchar_t * value, size_t);

z::core::string<z::utf8> operator"" _u8(char value);
z::core::string<z::utf8> operator"" _u8(wchar_t value);
z::core::string<z::utf8> operator"" _u8(const char *value);
z::core::string<z::utf8> operator"" _u8(const char *value, size_t);
z::core::string<z::utf8> operator"" _u8(const wchar_t * value, size_t);

z::core::string<z::ascii> operator"" _asc(char value);
z::core::string<z::ascii> operator"" _asc(wchar_t value);
z::core::string<z::ascii> operator"" _asc(const char *value);
z::core::string<z::ascii> operator"" _asc(const char *value, size_t);
z::core::string<z::ascii> operator"" _asc(const wchar_t * value, size_t);

zstring operator"" _zs(char value);
zstring operator"" _zs(wchar_t value);
zstring operator"" _zs(const char *value);
zstring operator"" _zs(const char *value, size_t);
zstring operator"" _zs(const wchar_t *value, size_t);
