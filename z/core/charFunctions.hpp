#pragma once

#include <cstdint>
#include <cstddef>

namespace z
{
	namespace core
	{
		/**
		 * \brief Check if the given character is an uppercase
		 * alphabetic character.
		 *
		 * This function differs from isUpper() in that it only
		 * returns true for characters `A-Z`.
		 *
		 * \param ch the character to check.
		 *
		 * \return True if the character is uppercase. False otherwise.
		 */
		bool isUpperAlpha(uint32_t ch) noexcept;

		/**
		 * \brief Check if the given character is a lowercase
		 * alphabetic character.
		 *
		 * This function differs from isLower() in that it only
		 * returns true for characters `a-z`.
		 *
		 * \param ch the character to check.
		 *
		 * \return True if the character is lowercase. False otherwise.
		 */
		bool isLowerAlpha(uint32_t ch) noexcept;

		/**
		 * \brief Check if the given character is an uppercase character.
		 *
		 * \param ch the character to check.
		 *
		 * \return True if the character is uppercase. False otherwise.
		 */
		bool isUpper(uint32_t ch) noexcept;

		/**
		 * \brief Check if the given character is a lowercase character.
		 *
		 * \param ch the character to check.
		 *
		 * \return True if the character is lowercase. False otherwise.
		 */
		bool isLower(uint32_t ch) noexcept;

		/**
		 * \brief Convert the given character to uppercase.
		 *
		 * \param ch the character to convert.
		 * \param camel If true, convert characters with a camelCase
		 * version (e.g. `Ǌ` vs `ǋ` vs `ǌ`). If false, convert only
		 * to uppercase.
		 *
		 * \return The character, converted to uppercase if applicable.
		 */
		uint32_t toUpper(uint32_t ch, bool camel = false) noexcept;

		/**
		 * \brief Convert the given character to lowercase.
		 *
		 * \param ch the character to convert.
		 * \param alternate If true, use whatever alternate lowercase version
		 * a character has, if any (e.g. `Σ`&rarr;`ς` instead of `Σ`&rarr;`σ`).
		 *
		 * \return The character, converted to lowercase if applicable.
		 */
		uint32_t toLower(uint32_t ch, bool alternate = false) noexcept;

		/**
		 * \brief Check if the given character is an
		 * alphabetic character.
		 *
		 * \param ch the character to check.
		 *
		 * \return \b True if the character is an alphabetic character.
		 * \b False otherwise.
		 */
		bool isAlpha(uint32_t ch) noexcept;

		/**
		 * \brief Convert the given character to its
		 * respective numeral value.
		 *
		 * '0'-'9' give 0-9. from 'A'-'Z' give 10-36.
		 * Case insensitive.
		 *
		 * \param ch the character to check.
		 *
		 * \return The numeral value of the character.
		 * If it is not a numeric character, returns -1.
		 */
		int numeralValue(uint32_t ch) noexcept;

		/**
		 * \brief Convert the given value to its
		 * respective numeral character.
		 *
		 * 0-9 give '0'-'9'. 10-36 give 'A'-'Z'.
		 *
		 * \param value the value to convert.
		 *
		 * \return The character numeral for the value.
		 * If it is not a numeral value, returns '0'.
		 */
		uint32_t numeral(int value) noexcept;

		/**
		 * \brief Check if the given character is numeric under the given base.
		 *
		 * \param ch The character to check.
		 * \param base The base for this character.
		 *
		 * \return True if the character is numeric under the given base. False otherwise.
		 */
		bool isNumeric(uint32_t ch, int base = 10) noexcept;

		/**
		 * \brief Check if the given character is alphanumeric.
		 *
		 * \param ch The character to check.
		 *
		 * \return True if the character is alphanumeric. False otherwise.
		 */
		bool isAlphaNumeric(uint32_t ch) noexcept;

		/**
		 * \brief Check if the given character is white space.
		 *
		 * \param ch The character to check.
		 *
		 * \return True if the character is a white space character. False otherwise.
		 */
		bool isWhiteSpace(uint32_t ch) noexcept;

		/**
		 * \brief Convert a character to UTF-8 encoding.
		 *
		 * \param c The UTF-8 sequence, returned by pointer. Assumed to be a 4-item array.
		 * \param chr The character to convert to UTF-8.
		 *
		 * \return The number of characters in the UTF-8 sequence.
		 */
		int toUTF8(uint8_t* c, uint32_t chr) noexcept;

		/**
		 * \brief Get the length of the UTF-8 encoding for a character.
		 *
		 * \param chr The character to convert to UTF-8.
		 *
		 * \return The number of characters in the UTF-8 sequence.
		 */
		int lenToUTF8(uint32_t chr) noexcept;


		/**
		 * \brief Convert a character from UTF-8 to UTF32 encoding.
		 *
		 * \param c The UTF-8 sequence. Assumed to be a 4-item array.
		 *
		 * \return The character encoded as UTF32. '?' if input is not valid UTF-8.
		 */
		uint32_t fromUTF8(uint8_t* c) noexcept;

		/**
		 * \brief Get the character length of a UTF-8 sequence.
		 *
		 * \param c The UTF-8 sequence. Assumed to be a 4-item array.
		 *
		 * \return The number of characters in the UTF-8 sequence. Assumed to be valid UTF-8.
		 */
		int lenFromUTF8(uint8_t* c) noexcept;

		/**
		 * \brief Get whether the characters are in valid UTF-8 format.
		 *
		 * \param c The character sequence. Assumed to have \b len characters.
		 * \param len The characters in the sequence. Characters after the 4th will be ignored.
		 *
		 * \return True if the sequence is valid UTF-8 format, false otherwise.
		 */
		bool isUTF8(uint8_t* c, int len) noexcept;
	}
}
